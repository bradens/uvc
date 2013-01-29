from django.db import models
from appointments.models import Event, Appointment, KennellingAppointment
from datetime import datetime

class ScheduleManager(models.Model):
	# Basically toString override
	def __unicode__(self):
		return "Schedule Manager"

	# Will return true if event can be fit into schedule
	@staticmethod
	def canfit(startTime,endTime,eventType):
		if(eventType == "APPOINTMENT"):
			start = datetime.strptime(startTime, "%Y-%m-%d %H:%M")
			end = datetime.strptime(endTime, "%Y-%m-%d %H:%M")
			x = 0
			for app in Appointment.objects.filter(start__lte=start, end__gte=end):
				try:
					k = app.kennellingappointment
				except KennellingAppointment.DoesNotExist:
					x += 1
			y = len(Shift.objects.filter(start__lte=start, end__gte=end))
			return x < y-1
		elif (eventType == "KENNELLING"):
			date = startTime
			x = len(KennellingAppointment.objects.filter(start__lte=date,end__gte=date))
			return x < 3
		elif(eventType == "SHIFT"):
			return true
		else:
			return 'type invalid'

	#calcHours
	def calcHours(startTime,endTime,id):
		h = 0
		for s in Shift.object.filter(start>=startTime,end<=endTime,employee_id==id):
			h+=s.get_duration()
		return h

	
	
	
	def getNumEvents(self):
		return len(Event.objects.all())

class Shift(Event):
	employee_id = models.CharField("employee ID", max_length=100)

	def getDuration(self):
		h = end-start
		return h.seconds/3600








