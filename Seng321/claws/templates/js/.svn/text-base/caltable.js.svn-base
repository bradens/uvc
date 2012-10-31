var calTable = function (element){
	this.jelement = $(element);
	this.tbody = this.jelement.find("tbody");
	
	// function that will be called on click to any datacell.
	// override this in your view with your custom functionality.
	this.dataCellClickHandler = function (e) {
	  console.log("default click handler.");
	}
	
	this.initEvents = function() {
	  this.jelement.find(".bookingsDayTableRow td:not(.timeCell)").click(this.dataCellClickHandler);
	}
	
	this.formatDate = function(date)
	{
		return date.getFullYear() + "-" + (date.getMonth()+1) + "-" + date.getDate();
	}
	
	this.rewrite = function(data){
		if (!data){ // initialize the table with empty values
			data = {shifts: []};
			for(var i = 0;i < 56;i++) data.shifts[i] = "";
		}
		this.jelement.find('.bookingsDayTableRow').remove();
		if (data.startDate)
		{
			this.jelement.find("tr:last").after(
					'<tr class="bookingsDayTableRow">' + 
					'<td class="timeCell">Date</td><td>' +
					data.calTable.formatDate(data.startDate) +
					'</td><td>' +
					data.calTable.formatDate((new Date(data.startDate.getTime() + 86400000*1))) +
					'</td><td>' +
					data.calTable.formatDate((new Date(data.startDate.getTime() + 86400000*2))) +
					'</td><td>' +
					data.calTable.formatDate((new Date(data.startDate.getTime() + 86400000*3))) +
					'</td><td>' +
					data.calTable.formatDate((new Date(data.startDate.getTime() + 86400000*4))) +
					'</td><td>' +
					data.calTable.formatDate((new Date(data.startDate.getTime() + 86400000*5))) +
					'</td><td>' +
					data.calTable.formatDate((new Date(data.startDate.getTime() + 86400000*6))) +
					'</td><td>&nbsp;</td></tr>');
		}
		if (data.is_appointments)
		{
			var k = 1;
			this.jelement.find("tr:last").after(
					'<tr class="bookingsDayTableRow">' + 
					'<td class="timeCell">Kennels</td><td>' +
					data.shifts[0] +
					'</td><td>' +
					data.shifts[1] +
					'</td><td>' +
					data.shifts[2] +
					'</td><td>' +
					data.shifts[3] +
					'</td><td>' +
					data.shifts[4] +
					'</td><td>' +
					data.shifts[5] +
					'</td><td>' +
					data.shifts[6] +
					'</td><td>&nbsp;</td></tr>');
		}
		else
			var k = 0;
		for (var i = 9; i < 17; i++) {
			this.jelement.find("tr:last").after(
				'<tr class="bookingsDayTableRow">' + 
				'<td class="timeCell">' + i +
				':30</td><td>' +
				data.shifts[0 + k*7] +
				'</td><td>' +
				data.shifts[1 + k*7] +
				'</td><td>' +
				data.shifts[2 + k*7] +
				'</td><td>' +
				data.shifts[3 + k*7] +
				'</td><td>' +
				data.shifts[4 + k*7] +
				'</td><td>' +
				data.shifts[5 + k*7] +
				'</td><td>' +
				data.shifts[6 + k*7] +
				'</td><td>&nbsp;</td></tr>');
			k++;
		}
		this.initEvents();
	}
}; 
