function logout() 
{
	CommHandler.doPost("/login/logout", null, onLogout);
}

function onLogout()
{
	window.location = "/";
}

var CommHandler = {
	getCookie: function(name) {
        var cookieValue = null;
        if (document.cookie && document.cookie != '') {
            var cookies = document.cookie.split(';');
            for (var i = 0; i < cookies.length; i++) {
                var cookie = jQuery.trim(cookies[i]);
                // Does this cookie string begin with the name we want?
                if (cookie.substring(0, name.length + 1) == (name + '=')) {
                    cookieValue = decodeURIComponent(cookie.substring(name.length + 1));
                    break;
                }
            }
        }
        return cookieValue;
    },
		
	defaultErr: function(data, err, thrown) {
		console.log(err);
	},

	doPost : function(url, data, success, error)
	{
		if (!error)
			error = this.defaultErr;
		$.ajax({
			type: "POST",
			url: url,
			data: data,
			success: success,
			error: error,
			headers: {
				"X-CSRFToken": CommHandler.getCookie('csrftoken')
			},
			dataType: "json"
		});
	},

	doGet : function(url, data, success, error)
	{
		if (!error)
			error = this.defaultErr;
		$.ajax({
			type: "GET",
			url: url,
			data: data,
			success: success,
			error: error,
			headers: {
        "X-CSRFToken": CommHandler.getCookie('csrftoken')
      },
      dataType: "json"
		});
	}	
};
