var loginController = {
		init : function() {
			$(".loginForm input").keyup(function(e){
				var code = (e.keyCode ? e.keyCode : e.which);
				if(code == 13) { // Enter
					loginController.login();
				}
			});
			$(".registerForm input").keyup(function(e){
				var code = (e.keyCode ? e.keyCode : e.which);
				if(code == 13) { // Enter
					loginController.submit();
				}
			});
		},
		login: function()
		{
			var data = {
					username: $("#usrname").val(),
					password: $("#password").val()
			};
			CommHandler.doPost("/login/login", data, function(res) {
				if (res.err)
					$("#loginError")[0].innerHTML = res.err;
				if (res.url)
					window.location = res.url;
			});
		},
		forgotPass: function()
		{
			var data = {
			};
			CommHandler.doPost("/login/forgotPass", data, function(res) {
				if (res.url)
					window.location = res.url;
			});
		},
		forgotPassSent: function()
		{
			var data = {
					email: $("#email").val()
			};
			CommHandler.doPost("/login/forgotPassSent", data, function(res) {
				if (res.url)
					window.location = res.url;
			});
		},
		submit: function()
		{
			var data = {
					email: $("#email").val(),
					password: $("#regpassword").val(),
					cPassword: $("#cmfpassword").val()
			};
			CommHandler.doPost("/login/create", data, function(res) {
				if (res.url)
					window.location = res.url;
			});
		},
		editaccount_submit: function(usr)
		{
			if($("#password").val() == $("#password_confirm").val() && $("#email").val() != "")
			{
				var data = {
						user: usr,
						email: $("#email").val(),
						password: $("#password").val()
				};
				CommHandler.doPost("/login/editaccount", data, function(res) {
					if (res.url)
						window.location = res.url;
				});
			};
		},
		editUserAccountForm: function(usr)
		{
			var data = {
						user: usr
			};
			CommHandler.doPost("/login/editUserAccountForm", data, function(res) {
				if (res.url)
					window.location = res.url;
			});
		},
		activate: function(usr)
		{
			var data = {
					user: usr
			};
			CommHandler.doPost("/login/activate", data, function(res) {
				$("#" + usr + "-buttons")[0].innerHTML = "Confirmed:";
				if (res.url)
					window.location = res.url;
			});
		},
		deny: function(usr)
		{
			var data = {
					user: usr
			};
			CommHandler.doPost("/login/deny", data, function(res) {
				$("#" + usr + "-buttons")[0].innerHTML = "Denied:";
				if (res.url)
					window.location = res.url;
			});
		},
}
$(document).ready(function(){
	loginController.init();
});
