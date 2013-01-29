from django.conf.urls.defaults import patterns, include, url
from django.views.generic.create_update import create_object

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('dashboard.views', 
    url(r'^dash/$', 'dash'),
)

urlpatterns += patterns('scheduling.views', 
    url(r'^schedule/$', 'schedule'),
    url(r'^scheduling/getData', 'getData'),
    url(r'^schedule/(?P<event_id>\d+)/$', 'detail'),
    url(r'^schedule/create/$', 'create'),
    url(r'^schedule/(?P<event_id>\d+)/edit$', 'edit'),
    url(r'^schedule/(?P<event_id>\d+)/updateData$', 'update'),
    url(r'^schedule/(?P<event_id>\d+)/delete$', 'delete'),
    url(r'^schedule/(?P<event_id>\d+)/delete_confirmed$', 'delete_confirmed'),
		url(r'^schedule/getHours/', 'getHours'),
)

urlpatterns += patterns('users.views',
    url(r'^$', 'index'),
    url(r'^login/login$', 'login'),
    url(r'^login/forgotPassword$', 'forgotPass'),
    url(r'^login/forgotPassSent$', 'forgotPassSent'),
    url(r'^login/forgotPassSuccess$', 'forgotPassSuccess'),
    url(r'^login/logout$', 'logout'),
    url(r'^login/create$', 'newUser'),
    url(r'^login/created$', 'newUserSuccess'),
    url(r'^login/editaccount$', 'editAccount'),
    url(r'^login/editaccountForm$', 'editAccountForm'),
    url(r'^login/editUserAccounts$', 'editUserAccounts'),
    url(r'^login/editUserAccountForm/(?P<user>\d+)/$', 'editUserAccountForm'),
    url(r'^login/pending$', 'pendingUsers'),
    url(r'^login/activate$', 'activateUser'),
    url(r'^login/deny$', 'denyUser'),
    url(r'^employee/pending$', 'pendingUsers'),
    url(r'^initialize$', 'initialize',)
) 

urlpatterns += patterns('appointments.views', 
    url(r'^appointments/$', 'appointment_list'),
    url(r'^appointments/(?P<event_id>\d+)/$', 'detail'),
    url(r'^appointments/(?P<event_id>\d+)/log$', 'log'),
    url(r'^appointments/(?P<event_id>\d+)/updateLog$', 'updateLog'),
    url(r'^appointments/(?P<event_id>\d+)/catcam$', 'cam'),
    url(r'^appointments/(?P<event_id>\d+)/edit$', 'edit'),
    url(r'^appointments/(?P<event_id>\d+)/editsubmit$', 'editsub'),
    url(r'^appointments/(?P<event_id>\d+)/delete$', 'delete'),
    url(r'^appointments/(?P<event_id>\d+)/deleteConf$', 'deleteConfirmed'),
    url(r'^appointments/create$', 'create'),
    url(r'^appointments/create/new$', 'createnew'),
    url(r'^appointments/data/getGroomingAv$', 'getGroomingAvailability'),
    url(r'^appointments/data/getKennellingAv$', 'getKennellingAvailability'),
    url(r'^appointments/data/getKennellingAv2$', 'getKennellingAvailability2'),
    url(r'^appointments/data/getAppointmentList$', 'getAppointmentList'),
)

urlpatterns += patterns('catalogue.views',
    url(r'^catalogue/$', 'index'),
    url(r'^catalogue/getAllJson/$', 'getAllEntries'),
    url(r'^catalogue/storeEntry/$', 'storeEntry'),
    url(r'^catalogue/deleteEntry/$', 'deleteEntry'),
) 

urlpatterns += patterns('',
    # Examples:
    # url(r'^$', 'claws.views.home', name='home'),
    # url(r'^claws/', include('claws.foo.urls')),
    # Uncomment the admin/doc line below to enable admin documentation:
    # url(r'^admin/doc/', include('django.contrib.admindocs.urls')),
    # Uncomment the next line to enable the admin:
    url(r'^admin/', include(admin.site.urls)),
)
