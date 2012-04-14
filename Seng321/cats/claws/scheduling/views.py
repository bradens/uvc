from django.shortcuts import render_to_response, get_object_or_404
from scheduling.models import Shift, Appointment

def index(request):
	shift_list = Shift.objects.all().order_by('-start')
	return render_to_response('scheduling/index.html', {'shift_list': shift_list})

def detail(request, event_id):
	shift = get_object_or_404(Shift, pk=event_id)
	return render_to_response('scheduling/detail.html', {'shift': shift})

