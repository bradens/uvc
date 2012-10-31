from django.http import HttpResponse, HttpResponseRedirect, Http404, HttpResponseNotFound
from django.utils import simplejson
from django.shortcuts import render_to_response, get_object_or_404
from django.contrib.auth.models import User, Group
from catalogue.models import Item
from django.contrib import auth
from django.contrib.auth import authenticate, logout
from users.models import PendingUser
from django.contrib.auth.decorators import login_required, user_passes_test
from django.core.context_processors import csrf
from django.views.decorators.csrf import csrf_exempt

def index(request):
    if request.user.is_authenticated():
		return HttpResponseRedirect('/dash')
    return render_to_response('login/index.html', { 'items': Item.objects.all()})

def newUserSuccess(request):
    #TODO redirect to dashboard if logged in
    return render_to_response('login/created.html')

@login_required
@user_passes_test(lambda u: inManGroup(u) or inEmpGroup(u), login_url='')
def pendingUsers(request):
    return render_to_response('login/pending.html', {'users': [usr for usr in User.objects.all() if not usr.is_active]})

def forgotPass(request):
    return render_to_response('login/forgotPassword.html')

def forgotPassSent(request):
    response = {"url": "/login/forgotPassSuccess"}
    return HttpResponse(simplejson.dumps(response), mimetype="text/javascript")

def forgotPassSuccess(request):
    return render_to_response("login/forgotPassSent.html")
    
def logout(request):
    if (request.user.is_authenticated):
        auth.logout(request);
    return HttpResponseRedirect('/')

@csrf_exempt
def login(request):
    #TODO redirect to dashboard if logged in
    username = request.POST.get('username')
    pwd = request.POST.get('password')
    
    url = '/dash/'
    
    user = authenticate(username=username, password=pwd)
    if user is not None:
        if user.is_active:
            response = {'url': url}
        else:
            response = {'err' : "Account has not been activated" }
    else:
        response = {'err' : "Incorrect username or password" }
    if (response.get('url') != None):
        auth.login(request, user)
    return HttpResponse(simplejson.dumps(response), mimetype="text/javascript")


# These return true if the user is in the group
def inEmpGroup(user):
    if (user.groups.filter(name='employee').count() == 0):
        return False
    else:
        return True
def inCusGroup(user):
    if (user.groups.filter(name='customer').count() == 0):
        return False
    else:
        return True
def inManGroup(user):
    if (user.groups.filter(name='manager').count() == 0):
        return False
    else:
        return True
    
def setUserGroup(user, groupname):
    Group.objects.get(groupname=groupname).user_set.add(user)
    user.save()
	
@login_required
@user_passes_test(lambda u: inManGroup(u) or inEmpGroup(u) or inCusGroup(u), login_url='')
def editAccount(request):	
	user = request.POST.get('user','')
	actualUser = User.objects.get(pk = user)
	
	if request.POST.get('email', '') != "":
		actualUser.email = request.POST.get('email','')
	if request.POST.get('password', '') != "":
		actualUser.set_password(request.POST.get('password', ''))
	
	actualUser.save();
		
	response = {}
	if inEmpGroup(request.user) or inManGroup(request.user):
		response.update({'url': '/login/editUserAccounts'})
	else:
		response.update({'url': '/dash'})
	return HttpResponse(simplejson.dumps(response), mimetype="text/javascript")

# For customers: only edit their own account
@login_required
@user_passes_test(lambda u: inCusGroup(u), login_url='')
def editAccountForm(request):
    #TODO redirect to dashboard if logged in
    return render_to_response('login/edit_account.html', {'user': request.user, 'cancel_link': '/dash'})

# For employee: edit any account
@login_required
@user_passes_test(lambda u: inManGroup(u) or inEmpGroup(u), login_url='')
def editUserAccountForm(request, user):
    #TODO redirect to dashboard if logged in
    actualUser = User.objects.get(pk = user)
    return render_to_response('login/edit_account.html', {'user': actualUser, 'cancel_link': '/login/editUserAccounts'})

@login_required
@user_passes_test(lambda u: inManGroup(u) or inEmpGroup(u), login_url='')
def editUserAccounts(request):
    if inManGroup(request.user):
        users = [usr for usr in User.objects.all()]
    else:
        users = [usr for usr in User.objects.all() if inCusGroup(usr)]
    return render_to_response('login/editUserAccounts.html', {'users': users})

def newUser(request):
    if request.method == 'POST':
        email = request.POST.get('email', '')
        password = request.POST.get('password', '')
#	cPassword = request.POST.get('cPassword', '')
    
	#check desired username against existing usernames
	#if password == cPassword:
        user = User.objects.create_user(email, email, password)
        user.is_active = False
        Group.objects.get(name="customer").user_set.add(user)

        user.save()
    
        response = {}
        response.update({'url': '/login/created'})
        return HttpResponse(simplejson.dumps(response), mimetype="text/javascript")
    else:
        return HttpResponseNotFound()
			
@login_required
@user_passes_test(lambda u: inManGroup(u) or inEmpGroup(u), login_url='')
def activateUser(request):
    if request.method == 'POST':
		userid = request.POST.get('user')
		user = User.objects.get(pk = userid)
		user.is_active = True
		user.save()

    response = {}
    return HttpResponse(simplejson.dumps(response), mimetype="text/javascript")

@login_required
@user_passes_test(lambda u: inManGroup(u) or inEmpGroup(u), login_url='')
def denyUser(request):
    if request.method == 'POST':
		userid = request.POST.get('user')
		user = User.objects.get(pk = userid)
		user.delete()

    response = {}
    return HttpResponse(simplejson.dumps(response), mimetype="text/javascript")

# NOTE -- THIS METHOD IS JUST HERE TO CREATE USER DATA
# COMMENT OUT FOR PRODUCTION CODE
def initialize(request):
    group = Group(name="customer")
    group.save()
    group = Group(name="manager")
    group.save()
    group = Group(name="employee")
    group.save()
    
    username = 'emp1'
    email = 'emp1'
    password = 'emp1'
    user = User.objects.create_user(username, email, password)
    Group.objects.get(name="employee").user_set.add(user)
    user.save()
    
    username = 'man1'
    email = 'man1'
    password = 'man1'
    user = User.objects.create_user(username, email, password)
    Group.objects.get(name="manager").user_set.add(user)
    user.save()
    
    username = 'cus1'
    email = 'cus1'
    password = 'cus1'
    user = User.objects.create_user(username, email, password)
    Group.objects.get(name="customer").user_set.add(user)
    user.save()
    
    return HttpResponse(status=200)
