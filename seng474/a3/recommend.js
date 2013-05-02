var _ 			= require('underscore'),
		critics = require('./critics.json');


var criticsArray = [];
function getAverage(coll) {
	var x_ = 0, xCount = 0;
	_.each(coll, function(val, key) {
		x_ += val;
		xCount++;
	});
	return (x_ / xCount);
}

// Calculate the similarity between two users
function sim(x, y) {
	var x_ = 0, y_ = 0, sum1 = 0, sum2 = 0, sum3 = 0;
	itemsRatedByX = critics[x];
	itemsRatedByY = critics[y];
	x_ = getAverage(itemsRatedByX);
	y_ = getAverage(itemsRatedByY);

	_.each(itemsRatedByX, function(val, key) {
		fp = (val - x_);
		var sp = 0;
		if (!_.isUndefined(itemsRatedByY[key])) {
			sp = (itemsRatedByY[key] - y_);
		}
		else {
			// Skip this one.
		}
		sum1 += fp * sp;
	});

	_.each(itemsRatedByX, function(val, key) {
		sum2 += Math.pow((val - x_), 2);
	});

	_.each(itemsRatedByY, function(val, key) {
		sum3 += Math.pow((val - y_), 2);
	});

	return (sum1 / (Math.sqrt(sum2) * Math.sqrt(sum3)));
}

var double_r = function(user, item) {
	// Given user and item, gives the prediction rating u would give to i
	// todo
	if (critics[user][item]) {
		console.log(user + ": " + item + " = " + critics[user][item]);
	}
	else {
		// Predict
		console.log("Braden Simpson, V00685500.\nRecommender system with Pearson Correlation and user-user similarities.");
		console.log("--------------------------------------------------------------------\nPredicting...");
		var sum1 = 0, sum2 = 0;
		usersRated = {};
		_.each(critics, function(val, key) {
			if (!_.isUndefined(val[item])) {	
				var userObj = {};
				userObj[key] = val;
				_.extend(usersRated, userObj);
			}
		});

		_.each(usersRated, function(val, key) {
			sum1 += val[item] * sim(key, user);
			// console.log(user);
			sum2 += sim(key, user);
		});

		p = (sum1/sum2);
		console.log("Predicted: " + user + " -- " + item + " = " + p);
	}
}

if (process.argv.length < 4) {
	console.log("Usage: node recommender.js [user] [item]");
}
else {
	var user = process.argv[2];
	var item = process.argv[3];
	double_r(user, item);
}