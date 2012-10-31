function timePicker(existing_value)
{
	this.scrollRight = function()
	{
		this.current = new Date(this.current.getTime() + 86400000);
		this.updateHeaders();
		this.moveAvailability("right");
	}
	this.scrollLeft = function()
	{
		new_date = new Date(this.current.getTime() - 86400000);
		if (new_date > (new Date()))
		{
			this.current = new_date;
			this.updateHeaders();
			this.moveAvailability("left");
		}
	}
	
	this.resetSelected = function()
	{
		$(document).getElementById("tpTmeDspYr").innerHTML = "####";
		$(document).getElementById("tpTmeDspMn").innerHTML = "##";
		$(document).getElementById("tpTmeDspDy").innerHTML = "##";
		$(document).getElementById("tpTmeDspHr").innerHTML = "##:##";
		this.last_selected.className = "timePickerField";
		this.last_selected = "";
		$(document).getElementById("start_time").value = "";
		$(document).getElementById("end").value = "";
	}
	this.moveAvailability = function(dir)
	{
		// shift everything left (right arrow was pressed)
		if (dir == "right")
		{
			to = "lft ";
			from = "rght";
		}
		else
		{
			to = "rght";
			from = "lft ";
		}
		for(i = 1;i < 4;i++)
		{
			for(k = 0;k < 4;k++)
			{
				if (i == 1)
				{
					dispTime = k + 9;
				}
				else if (i == 2)
				{
					dispTime = k + 1;
				}
				else
				{
					dispTime = k + 5;
				}
				if ($(to+dispTime).className == "timePickerFieldSelected")
				{
					this.resetSelected();
				}
				$(to+dispTime).className = $("cntr"+dispTime).className;
				if ($(to+dispTime).className == "timePickerFieldSelected")
				{
					this.last_selected = $(to+dispTime);
				}
				$("cntr"+dispTime).className = $(from+dispTime).className;
				if ($("cntr"+dispTime).className == "timePickerFieldSelected")
				{
					this.last_selected = $("cntr"+dispTime);
				}
			}
		}
		this.updateAvailability(from);
	}
	this.updateAvailability = function(col)
	{
		if (col == "lft ")
		{
			if (this.last_selected == "" && this.existing_date != "" && this.existing_date.getDate() == (new Date(this.current.getTime() - 86400000)).getDate() &&
				this.existing_date.getFullYear() == this.current.getFullYear() &&
				this.existing_date.getMonth() == this.current.getMonth())
			{
				time = parseInt(this.existing_selected.id.substring(4));
				this.newTime(this.existing_selected, time, "lft ");
			}
		}
		else if (col == "rght")
		{
			if (this.last_selected == "" && this.existing_date != "" && this.existing_date.getDate() == (new Date(this.current.getTime() + 86400000)).getDate() &&
				this.existing_date.getFullYear() == this.current.getFullYear() &&
				this.existing_date.getMonth() == this.current.getMonth())
			{
				time = parseInt(this.existing_selected.id.substring(4));
				this.newTime(this.existing_selected, time, rght);
			}
		}
		else
		{
			if (this.last_selected == "" && this.existing_date != "" && this.existing_date.getDate() == this.current.getDate() &&
				this.existing_date.getFullYear() == this.current.getFullYear() &&
				this.existing_date.getMonth() == this.current.getMonth())
			{
				time = parseInt(this.existing_selected.id.substring(4));
				this.newTime(this.existing_selected, time, rght);
			}
		}
		for (i = 1;i < 4;i++)
		{
			for(k = 0;k < 4;k++)
			{
				if (col == "lft ")
				{
					start = this.getLeftDateString();
					end = this.getLeftDateString();
				}
				else if (col == "cntr")
				{
					start = this.getCenterDateString();
					end = this.getCenterDateString();
				}
				else if (col == "rght")
				{
					start = this.getRightDateString();
					end = this.getRightDateString();
				}
				if (i == 1)
				{
					dispTime = k + 9;
					id = col + dispTime;
				}
				else if (i == 2)
				{
					dispTime = k + 13;
					id = col + (dispTime - 12);
				}
				else
				{
					dispTime = k + 17;
					id = col + (dispTime - 12);
				}
				start += " " + dispTime + ":30";
				end += " " + (dispTime + 1) + ":30";
				var jqhr = jQuery.get("/appointments/data/getGroomingAv", 
					{ startTime : start, endTime : end, elementId : id })
				.success(function (data) {
						d = data.split(":");
						if (d[1] == "False" && 
							($(document).tp.last_selected != $(document).tp.existing_selected || 
							 $(d[0]) != $(document).tp.existing_selected))
						{
							$(d[0]).className = "timePickerFieldUnavailable";
							if ($(document).tp.last_selected == $(d[0]))
							{
								$(document).tp.resetSelected();
							}
						}
						else
						{
							if ($(document).tp.last_selected == $(d[0]))
							{
								$(d[0]).className = "timePickerFieldSelected";
							}
							else
							{
								$(d[0]).className = "timePickerField";
							}
						}
				});
			}
		}
	}
	
	this.updateHeaders = function()
	{
		$(document).getElementById("tpLftClmnHead").innerHTML = (this.getLeftColumnHeader());
		$(document).getElementById("tpCntrClmnHead").innerHTML = (this.getCenterColumnHeader());
		$(document).getElementById("tpRghtClmnHead").innerHTML = (this.getRightColumnHeader());
	}
	
	this.getLeftDateString = function()
	{
		return (new Date(this.current.getTime() - 86400000)).strftime("%Y-%m-%d");
	}
	this.getCenterDateString = function()
	{
		return this.current.strftime("%Y-%m-%d");
	}
	this.getRightDateString = function()
	{
		return (new Date(this.current.getTime() + 86400000)).strftime("%Y-%m-%d");
	}
	this.getLeftColumnHeader = function()
	{
		return (new Date((this.current).getTime() - 86400000)).toDateString();
	}
	this.getCenterColumnHeader = function()
	{
		return this.current.toDateString();
	}
	this.getRightColumnHeader = function()
	{
		return (new Date((this.current).getTime() + 86400000)).toDateString();
	}
	
	this.newTime = function(el, num, col)
	{
		if (col == "lft")
			date = (new Date((this.current).getTime() - 86400000));
		else if (col == "cntr")
			date = this.current;
		else
			date = (new Date((this.current).getTime() + 86400000))
			
		if (num < 9)
			num += 12;
			
		if (this.last_selected != "")
			this.last_selected.className = "timePickerField";
		el.className = "timePickerFieldSelected";
		this.last_selected = el;
		this.selected = new Date(date.getFullYear(), date.getMonth(), date.getDate(), num);
		this.updateSelected();
	}
	
	this.updateSelected = function()
	{
		$(document).getElementById("tpTmeDspYr").innerHTML = this.selected.getFullYear();
		$(document).getElementById("tpTmeDspMn").innerHTML = this.selected.getMonth() + 1;
		$(document).getElementById("tpTmeDspDy").innerHTML = this.selected.getDate();
		$(document).getElementById("tpTmeDspHr").innerHTML = ((this.selected.getHours() > 12) ? (this.selected.getHours() - 12) + ":30 PM" : (this.selected.getHours() + ":30 AM"));
		$(document).getElementById("start_time").value = this.selected.getFullYear() + "-" + (this.selected.getMonth() + 1) + "-" + this.selected.getDate() + " " + this.selected.getHours() + ":30";
		end_time = new Date(this.selected.getTime() + 3600000);
		$(document).getElementById("end").value = end_time.getFullYear() + "-" + (end_time.getMonth() + 1) + "-" + end_time.getDate() + " " + end_time.getHours() + ":30";
	}
	this.parseExistingValue = function(val)
	{
		dt = val.split(" ");
		dtd = dt[0].split("-");
		dtt = dt[1].split(":");
		d = new Date();
		d.setFullYear(parseInt(dtd[0]), parseInt(dtd[1]) - 1, parseInt(dtd[2]));
		d.setHours(parseInt(dtt[0]), parseInt(dtt[1]));
		this.existing_date = d;
		if (d.getFullYear() > this.current.getFullYear() ||
			d.getMonth() > this.current.getMonth() || 
			d.getDate() > this.current.getDate())
		{
			this.existing_selected = $("cntr" + d.getHours());
			this.current.setFullYear(d.getFullYear(), d.getMonth(), d.getDate());
			this.newTime(this.existing_selected, d.getHours(), "cntr");
		}
		else
		{
			this.existing_selected = $("lft " + d.getHours());
			this.current.setFullYear(d.getFullYear(), d.getMonth(), d.getDate() + 1);
			this.newTime(this.existing_selected, d.getHours(), "lft");
		}
		this.updateHeaders();
	}
	this.last_selected = "";//$("cntr11");
	this.selected = "";
	this.existing_selected = "";
	this.existing_date = "";
	this.current = new Date((new Date()).getTime() + 86400000);
	if (existing_value != undefined)
		this.parseExistingValue(existing_value);
	//this.newTime(this.last_selected, 11, "cntr");
	// initialize headers
	this.updateAvailability("lft ");
	this.updateAvailability("cntr");
	this.updateAvailability("rght");
	this.updateHeaders();
}

function generate()
{
	table = $(timeTable);
	for(i = 1;i < 4;i++)
	{
		row = table.insertRow(i + 1);
		for(k = 0;k < 14;k++)
		{
			cell = row.insertCell(k);
			if (k == 4 || k == 9)
			{
				cell.className = "timePickerFieldBorder";
			}
			else 
			{
				ak = (k < 4) ? k : ((k < 9) ? k - 1 : k - 2);
				if (i == 1)
				{
					dispTime = (ak % 4) + 9;
					ampm = "AM";
				}
				else if (i == 2)
				{
					dispTime = (ak % 4) + 1;
					ampm = "PM";
				}
				else
				{
					dispTime = (ak % 4) + 5;
					ampm = "PM";
				}
				
				if (ak < 4)
					col = 'lft ';
				else if (ak < 8)
					col = 'cntr';
				else
					col = 'rght';
				
				cell.className = "timePickerField";
				cell.id = col + dispTime;
				cell.innerHTML = '<span id="s' + col + dispTime + '">' + dispTime + ":30 " + ampm + "</span>";
				cell.onclick = function() {
					if (this.className != "timePickerFieldUnavailable")
					{
						col = this.id.substring(0, 4).trim();
						time = parseInt(this.id.substring(4));
						$(document).tp.newTime(this, time, col);
					}
				};
			}
		}
	}
}