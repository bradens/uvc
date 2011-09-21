#!/usr/bin/python
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#    Written by Chris Matthews cmatthew@cs.uvic.ca
#    Thanks to:
#        - Carl Sverre (carl@carlsverre.com) for regex based removal of prompt.

#only works in python 2.5, so check
import sys
if sys.version_info < (2, 3):
    raise "must use python 2.5 or greater"

"""Test a shell by feeding it input and checking the output"""
from optparse import OptionParser
import subprocess
import os
import string
import threading
import time
import signal
import sys
import re

def handler(signum, frame):
    print 'Segmentation Fault', signum
    sys.stdout.flush()
    exit(1)


signal.signal(signal.SIGPIPE, signal.SIG_DFL)
signal.signal(signal.SIGSEGV, handler)
__author__ = "Chris Matthews"
__version__ = "1.5"
options = None

globalrun = None;
globalprocess = None;

def fail():
    global globalrun
    global globalprocess
    print "failed."
    print globalrun
    if globalprocess != None:
        print "Actual Output:\n","\n".join(globalprocess.get_out_log()),
    sys.exit(1)

class Run:

    def __init__(self, name):
        self.__name = name
        self.__commands = []
        self.__outputs = []

    def add_inputs(self, command):
        self.__commands.append(command)

    def add_outputs(self, output):
        self.__outputs.append(output)

    def get_inputs(self):
        return self.__commands
    def get_name(self):
        return self.__name

    def get_out_log(self):
            return self.__outputs

    def __str__(self):
        return "Test: " + self.__name + "\nExecuted:\n" + string.join(self.__commands, "\n") + "\n\nExpected:\n" + string.join(self.__outputs, "\n")

class RunningProcess(threading.Thread):
    """Operations and data for individual processes running in the experiment.
    """
    def __init__(self, shell, input):
        """Create a new running process from this client.  """
        self.output = []
        threading.Thread.__init__(self)
        self.start_time = time.time()
        self.end_time = 0

        command = shell
        try:
            self.p = subprocess.Popen(command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.STDOUT,
                    shell=False,
                    close_fds=True,
                    bufsize=1024,
                   )
        except OSError:
            print "Shell file is missing, use -f parameter to set your shell name"
            exit(1)

        for line in input:
            self.p.stdin.write(line + "\n")

        self.p.stdin.close()

    def run(self):
        countdown = 1000
        while True:
            #self.output.append('\n'.join(self.p.stdout.readlines()))
            if self.is_dead():
                self.cleanup()
                return self.p.returncode
            if self.is_ended():
                self.cleanup()
                run_time = self.end_time - self.start_time
                self.run_time = run_time
                return self.p.returncode
            time.sleep(0.001)
            if (countdown % 100) == 0:
                print ".",
            countdown = countdown - 1
            if countdown == 0:
                print "\nProcess took too long."
                fail()

    def cleanup(self):
        """Collect all the logs etc and results"""

        self.output.extend(self.p.stdout.readlines())
        #(self.out, self.err) = self.p.communicate()
        self.p.stdout.close()

        self.end_time = time.time()

    def wait(self):
        """wrapper of process.wait()"""
        return self.p.wait()

    def is_dead(self):
        if (self.p.poll() != None):
            if (self.p.returncode != 0):
                return True
        return False

    def is_ended(self):
        if (self.p.poll() != None):
            if (self.p.returncode == 0):
                return True
        return False

    def poll(self):
        """wrapper of process.poll()"""
        return self.p.poll()

    def get_return_code(self):
        """wrapper of process.returncode"""
        return self.p.returncode

    def get_out_log(self):
        return self.output

    def get_pid(self):
        return self.p.pid
    def get_time(self):
        return self.end_time - self.start_time

def check_output(a, b):
    """Remove the prompt, and check if the two strings are equal less whitespace differences.
    @param a: a string
    @param b: a string
    @return: true if they are equal except for the prompt and whitespace differences
    """
    global options
    prompt_regex = options.regex

    new_a = []
    new_b = []

    for line in a:
        new_line = re.sub(prompt_regex, '', line)
        new_line = re.sub('\s','', new_line)

        if len(new_line) > 0:
            new_a.append(new_line)

    for line in b:
        new_line = re.sub(prompt_regex, '', line)
        new_line = re.sub('\s','', new_line)

        if len(new_line) > 0:
            new_b.append(new_line)

    #print "\na",a,"bnew",new_b,"b",b
    return new_a == new_b

def run_tester():
    """Run the shell tester with the given arguments."""
    global __version__
    global options
    global globalrun
    global globalprocess

    print "CSc360 Shell Tester: ", __version__
    parser = OptionParser()
    parser.add_option("-f", "--file", dest="shell_file",
                      help="use FILE as the name of the shell to test",
                      metavar="FILE",
                      default="ssi")

    parser.add_option("-i", "--in", dest="input",
                      help="the input commands are in FILE",
                      metavar="FILE",
                      default="ssi.in")

    parser.add_option("-r", "--regex", dest="regex",
                      help= "A regex pattern which matches your prompt, leave blank to match a prompt like 'SSI: / \$'",
                      default="SSI: / \$")

    (options, args) = parser.parse_args()

    print "Running shell named: ", options.shell_file
    try:
        inputs = open (options.input, "r")
    except IOError:
        print "Problem opening input file. try the -i argument."
        exit(2)
    runs = []
    current_run = None;
    input = True #are we in inputs or outputs mode?
    for line in inputs:
        if line.startswith("#"):
            current_run = Run(line[1:])
            runs.append(current_run)
            input = True

        elif line.startswith("-"):
            input = False
        else:
            if input:
                current_run.add_inputs(line.strip())
            else:
                current_run.add_outputs(line.strip())

    for run in runs:
        globalrun = run
        print "Test: ", run.get_name().strip(), "...",
        sys.stdout.flush()
        process = RunningProcess(options.shell_file, run.get_inputs())
        globalprocess = process
        process.run()
        if check_output(run.get_out_log(), process.get_out_log()):
            print "passed in %.4f sec." % process.get_time()
        else:
            fail()
    print "All tests passed!"

if __name__ == '__main__':
    run_tester();
