#Copyright Jon Berg , turtlemeat.com

import string,cgi,time
from os import curdir, sep
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer
#import psycopg2
import _pg
import subprocess
import json
#import pri

conn = _pg.connect("postgres", "localhost", 5432, None, None, "postgres", "something")
cur = conn.cursor()

class MyHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        try:
            if self.path == "/assign2j":
                print "assign2j"
                print "Running question 2. J)...."
                pgqueryset = conn.query("select * from discoverychannel_j()")
                result = pgqueryset.getresult()
                #cur.execute("select * from discoverychannel_j()")
                #result = '["directors" : ['
                    #for record in cur:
                    #result += '"' + record[0] + '"'
                    #if cur.rownumber != cur.rowcount:
                #    result += ","
                #result += "]]"
                #print "Directors\n" + result
                self.send_response(200)
                self.send_header('Content-type',	'text/json')
                self.end_headers()
                self.wfile.write(result)
            elif self.path == "/assign2r":
                print "assign2r"
                print "Running question 2. r)...."
                cur.execute("select * from discoverychannel_r()")
                result = '[{"title" : ' + record[0] + ', "count" : ' + str(record[1]) + '}]'
                print result
                self.send_response(200)
                self.send_header('Content-type',	'text/json')
                self.end_headers()
                self.wfile.write(result)
            else:
                if self.path.endswith(".html"):
                    print curdir + self.path
                    f = open(curdir + self.path) 
                    self.send_response(200)
                    self.send_header('Content-type',	'text/html')
                    self.end_headers()
                    self.wfile.write(f.read())
                    f.close()
                    return
            return
        
        except IOError:
            self.send_error(404,'File Not Found: %s' % self.path)
    
    def do_POST(self):
        global rootnode
        try:
            ctype, pdict = cgi.parse_header(self.headers.getheader('content-type'))
            if ctype == 'multipart/form-data':
                query=cgi.parse_multipart(self.rfile, pdict)
            self.send_response(301)
            
            self.end_headers()
            upfilecontent = query.get('upfile')
            print "filecontent", upfilecontent[0]
            self.wfile.write("<HTML>POST OK.<BR><BR>");
            self.wfile.write(upfilecontent[0]);
        
        except :
            pass

def main():
    try:
        server = HTTPServer(('', 8000), MyHandler)
        print 'started httpserver...'
        server.serve_forever()
    except KeyboardInterrupt:
        print '^C received, shutting down server'
        server.socket.close()
        cur.close()
        conn.close()

if __name__ == '__main__':
    main()

