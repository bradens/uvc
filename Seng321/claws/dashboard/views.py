from django.shortcuts import render_to_response, get_object_or_404
from users.views import inEmpGroup, inManGroup
from django.contrib.auth.decorators import login_required, user_passes_test

@login_required
def dash(request):
    u = request.user;
    if (inEmpGroup(u)):
        return render_to_response('dashboard/employee.html')
    elif (inManGroup(u)):
        return render_to_response('dashboard/manager.html')
    else:
        return render_to_response('dashboard/customer.html')
