var appointmentController = {
	calTable: null,
	init : function() {
		appointmentController.current = new Date();
		appointmentController.calTable = new calTable($(".appTable")[0]);
		appointmentController.calTable.rewrite();
	},
	
	getData : function(startDay, endDay) {
		var queryObj = jQuery.get("/appointments/data/getAppointmentList", 
						{ start : startDay.getFullYear() + "-" + (startDay.getMonth() + 1) + "-" + startDay.getDate(), 
						end : endDay.getFullYear() + "-" + (endDay.getMonth() + 1) + "-" + endDay.getDate() });
		queryObj.success(function (data) {
			d = 0;
		    dat = data.split(",");
		    new_table = new Array();
		    for (k = 0;k<63;k++)
		    	new_table[k] = ""
		    
		    for (i = 0; i < dat.length;i++)
		    {
		    	// new day
		    	app = dat[i].split("_");
		    	day = dat[i].split("-");
		    	if (app[0].length > 10)
		    		app[0] = app[0].substring(0,8) + "...";
		
		    	if (day[0] == "day")
		    	{
		    		d = parseInt(day[1], 10);
		    	}
		    	//kennelling appointment
		    	else if (app.length == 2)
		    	{
		    		new_table[d] += '<a class="listLink" href="/appointments/' + app[1] + '/">' + app[0] + '</a><br>';
		    	}
		    	// grooming appointment
		    	else if (app.length == 3)
		    	{
		    		new_table[d+(7*(parseInt(app[1], 10)-8))] += '<a class="listLink" href="/appointments/' + app[2] + '/">' + app[0] + '</a><br>';
		    	}
		    }
		    appointmentController.calTable.rewrite({ calTable: appointmentController.calTable, startDate: appointmentController.weekStart, shifts : new_table, is_appointments : true});
		});
	},
	
	refresh : function() {
		this.weekStart = new Date(this.current.getTime() - 86400000*(this.current.getDay()));
		this.weekEnd = new Date((this.weekStart).getTime() + 86400000*7);
		appointmentController.getData(this.weekStart, this.weekEnd);
	},
	
	scrollRight : function() {
		this.current = new Date(this.current.getTime() + 86400000*7);
		this.refresh();
	},
	
	scrollLeft : function() {
		this.current = new Date(this.current.getTime() - 86400000*7);
		this.refresh();
	},
}
$(document).ready(function() {
	this.getElementById("scrollRight").onclick = function()
	{
		appointmentController.scrollRight();
	}
	this.getElementById("scrollLeft").onclick = function()
	{
		appointmentController.scrollLeft();
	}
	appointmentController.init();
	appointmentController.refresh();
});
