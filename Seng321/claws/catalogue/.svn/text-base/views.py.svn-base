from django.http import HttpResponse
from django.shortcuts import render_to_response, get_object_or_404
from django.views.decorators.csrf import csrf_exempt
from catalogue.models import Item
from django.core.serializers import serialize
from django.utils import simplejson
from users.views import inEmpGroup, inManGroup
from django.contrib.auth.decorators import login_required, user_passes_test

def index(request):
	items = Item.objects.all()
	if (inEmpGroup(request.user)):
		return render_to_response('catalogue/emp.html', {'items': items, 'showActions': True });
	elif (inManGroup(request.user)):
		return render_to_response('catalogue/man.html', {'items': items, 'showActions': True });
	else:
		return render_to_response('catalogue/index.html', {'items': items });

@login_required
@user_passes_test(lambda u: inEmpGroup(u) or inManGroup(u), login_url='/')
def storeEntry(request):
	price = request.POST.get('price')
	description = request.POST.get('description')
	name = request.POST.get('name')
	imageURL = request.POST.get('imageURL')
	newItem = Item(name=name, description=description, price=price, imageURL=imageURL)
	newItem.save()
	response = [newItem]
	return HttpResponse(serialize("json", response), mimetype="text/javascript")

@login_required
@user_passes_test(lambda u: inEmpGroup(u) or inManGroup(u), login_url='/')
def deleteEntry(request):
	id = request.POST.get('id')
	Item.objects.get(pk=id).delete()
	response = {}
	response.update({'id': id })
	return HttpResponse(simplejson.dumps(response), mimetype="text/javascript")

def getAllEntries(request):
	response = Item.objects.all()
	return HttpResponse(serialize("json", response), mimetype="text/javascript")
	
def getEntry(request):
	id = request.POST.get('id')
	response = [Item.objects.get(pk=id)]
	return HttpResponse(serialize("json", response), mimetype="text/javascript")
	
