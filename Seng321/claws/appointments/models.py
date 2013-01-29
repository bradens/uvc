from django.db import models
from django.contrib.auth.models import User

date_format_string = "%Y-%m-%d %H:%M"

class Event(models.Model):
	EVENT_TYPES =  (
		('a', 'APPOINTMENT'),
		('k', 'KENNELING'),
		('s', 'SHIFT'),
	)
	eventType = models.CharField(max_length=1, choices=EVENT_TYPES)
	start = models.DateTimeField('start time')
	end = models.DateTimeField('end time')

	# Return duration in hours
	def get_duration(self):
		return (self.end - self.start).seconds / 3600

class Appointment(Event):
	customer_id = models.ForeignKey(User)
	cat_name = models.CharField(max_length=50)
	cat_breed = models.CharField(max_length=50)
	special_considerations = models.TextField()
	
	def is_kennelling(self):
		return False

	def __unicode__(self):
		return "(Grooming) " + "'" + self.cat_name + "'" + " from " + self.start.strftime(date_format_string) + " to " + self.end.strftime(date_format_string)

class KennellingAppointment(Appointment):
	log = models.TextField()
	camera_id = models.SmallIntegerField()
	camera_available = models.BooleanField()
	
	def is_kennelling(self):
		return True

	def __unicode__(self):
		return "(Kenneling) " + "'" + self.cat_name + "'" + " from " + self.start.strftime(date_format_string) + " to " + self.end.strftime(date_format_string)
