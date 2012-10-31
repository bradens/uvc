var scheduleController = {
	calTable: null,
    
    week:{},

	init : function() {
        scheduleController.current = new Date();
		scheduleController.calTable = new calTable($(".shiftTable")[0]);
		scheduleController.calTable.rewrite();
	},

	week : {},

	dates: {},
	
	getData : function(startDate,endDate) {
		this.week.weekStart = startDate.getFullYear() + "-" + (startDate.getMonth() + 1) + "-" + startDate.getDate() + " 8:30";
        this.week.weekEnd = endDate.getFullYear() + "-" + (endDate.getMonth() + 1) + "-" + endDate.getDate()  + " 17:30";

		this.dates.start = startDate;
		this.dates.end = endDate;

        var queryObj = jQuery.get("/scheduling/getData/", this.week);
        
        queryObj.success(function (data) {
            new_table = new Array();
		    for (k = 0;k<56;k++)
            {
		    	new_table[k] = "";
            }

            shifts = data.split(",");
            j = 0;
            for(i=0;i<shifts.length;i++)
            {
                sec = shifts[i].split("_");
                if(sec[0]=='day')
                {    
                    j = sec[1];
                    console.log("j = "+j+"\n");
                }
                else
                {
                    new_table[(j*8)+((sec[0]-j-9)*7)]+='<a class="listLink" href="/schedule/'+sec[2]+'/">'+sec[1]+'</a><br/>';
                    console.log("writing to cell "+((j*8)+((sec[0])*7))+"the reference "+ "<a href=\"/schedule/\""+sec[2]+"/\">"+sec[1]+"</a>" +"\n");
                }
            }
            scheduleController.calTable.rewrite({ calTable: scheduleController.calTable, startDate: scheduleController.dates.start, shifts : new_table});
        });
    },

	fillTable : function(){
		scheduleController.calTable
	},
     
    params : {},
    
    getHours : function(){
        
       
        this.params.startTime = $("#fromDate")[0].value;
        this.params.endTime = $("#toDate")[0].value;
        this.params.empId = $("#calculateEmpId")[0].value;
        console.log(this.params);
        
        var queryObj = jQuery.get("/schedule/getHours/", this.params);
        
        queryObj.success(function (data) {
            $("#calculated_hours")[0].value = data;
        });
        
        console.log(queryObj);
    },
    
  refresh : function() {
		this.weekStart = new Date(this.current.getTime() - 86400000*(this.current.getDay()));;
		this.weekEnd = new Date((this.weekStart).getTime() + 86400000*7);
        console.log("at inception: "+this.weekStart+" & "+this.weekEnd);
		scheduleController.getData(this.weekStart, this.weekEnd);
	},
	
	scrollRight : function() {
		this.current = new Date(this.current.getTime() + 86400000*7);
		this.refresh();
	},
	
	scrollLeft : function() {
		this.current = new Date(this.current.getTime() - 86400000*7);
		this.refresh();
	},
	
	createShift : function() {
	  CommHandler.doPost("/schedule/create/", {
	    employee_id: $("#employee_id").val(),
	    startTime: $("#date").val() + " " + $("#startTime").val(),
	    endTime: $("#date").val() + " " + $("#endTime").val()
	  },
	  function(data) {
	    scheduleController.refresh();
	  }); 
	}
};

$(document).ready(function() {
this.getElementById("scrollRight").onclick = function()
	{
		scheduleController.scrollRight();
	}
	this.getElementById("scrollLeft").onclick = function()
	{
		scheduleController.scrollLeft();
	}

	scheduleController.init();
    scheduleController.refresh();
});

