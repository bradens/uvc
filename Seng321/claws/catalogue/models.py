from django.db import models

# Create your models here.
class Item(models.Model):
	description = models.CharField(max_length=10000)
	name = models.CharField(max_length=100)
	price = models.FloatField()
	imageURL = models.CharField(max_length=100)