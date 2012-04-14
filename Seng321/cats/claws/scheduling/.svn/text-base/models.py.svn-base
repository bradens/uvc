from django.db import models

class ScheduleManager(models.Model):
	# Basically toString override
	def __unicode__(self):
		return "Schedule Manager"

	# Will return true if event can be fit into schedule
	def canFit(self, Event):
		return true

	def getNumEvents(self):
		return len(Event.objects.all())

class Event(models.Model):
	schedule_manager = models.ForeignKey(ScheduleManager)
	start = models.DateTimeField('start time')
	end = models.DateTimeField('end time')

	# Return duration in hours
	def get_duration(self):
		return (self.end - self.start).seconds / 3600

class Shift(Event):
	employee_id = models.CharField("employee ID", max_length=100)

class Appointment(Event):
	# customer = models.ForeignKey(Customer)
	cat_name = models.CharField(max_length=100)
	special_considerations = models.CharField(max_length=300)

#class KennelingAppointment(Appointment):
	# log
	# catcam number
