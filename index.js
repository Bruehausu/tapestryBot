console.log('beginning twitter bot');

var Twit = require('twit');
var fs = require('fs');
var parse = require('csv-parse');

var T = new Twit({
    consumer_key:         'OAVrmjtmDQJcRiQ21W5KWyKQh'
  , consumer_secret:      'auEN8XvPtkXTChgsLqP4nzMoPA8HPpGCJNpwgpr5mJET3nnK4C'
  , access_token:         '4849815927-dQ22J9qnRKzVbzJm0Ehas4gkcFAE9Ly2a0xiyMw'
  , access_token_secret:  'OGXVBaCJ04IcfhOtyJTzp4jdzEOaHTFe1cjAKIl7HOaM0'
})

var data = fs.readFileSync('matchings.csv');
var content_string = data.toString();
//console.log(content_string);
var matchings;
var curr_img;
var curr_img_ind;
var visited_imgs;

parse(content_string, {comment: '#'}, function(err, output){
	if(err){
		console.log("there was an error");
	}
	console.log("parsed length: " + output.length);
	matchings = output;
	setup();
	postImage();
	//setInterval(postImage, 1000*60*60);
	setInterval(postImage, 1000*60*60);
});

function setup(){
	console.log("matchings length: " + matchings.length);
	curr_img_ind = Math.floor(Math.random() * matchings.length);
	console.log("inital index: " + curr_img_ind);
	curr_img = matchings[curr_img_ind][0];
	visited_imgs = [curr_img];
}

function search(list, subj){
	for(var i = 0; i < list.length; i++){
		if (list[i] == subj){
			return true;
		}
	}
	return false;
}

function find_next(){
	var to_next;
	if(matchings[curr_img_ind][1] == "0" || 
	   search(visited_imgs, matchings[curr_img_ind][1])){
		if(matchings[curr_img_ind][2] == "0" || 
		   search(visited_imgs, matchings[curr_img_ind][2])){
			if(matchings[curr_img_ind][3] == "0" ||
			   search(visited_imgs, matchings[curr_img_ind][3])){
				console.log("found no valid follow");
				setup();
			} else {
				to_next = matchings[curr_img_ind][3];
			}
		} else {
			to_next = matchings[curr_img_ind][2];
		}
	} else {
		to_next = matchings[curr_img_ind][1];
	}

	for(var i = 0; i < matchings.length + 1; i++){
		if(i == matchings.legnth){
			console.log("failed to resolve partner");
			setup();
		} else if(matchings[i][0] == to_next){
			curr_img = to_next;
			curr_img_ind = i;
			visited_imgs.push(curr_img); 
			break;
		}
	}
}

function postImage(){

	var num = parseInt(curr_img, 10);
	var folder = (Math.floor(num / 1000)).toString();
	while(folder.length < 5){
		folder = "0" + folder;
	}
	var file = num.toString()
	while(file.length < 7){
		file = "0" + file;
	}


	var filename = "imageMatcher/bin/data/flickr/" + folder + "/" + file + ".jpg";

	var params = {
		encoding: 'base64'
	}

	var b64Content = fs.readFileSync(filename, params);

	console.log(filename);
	T.post('media/upload', { media_data: b64Content }, uploaded);

	//find_next();

	function uploaded(err, data, response) {
		if (err){
			console.log("post error");
		} else {
			//console.log("image" + curr_img + "uploaded");
			var id = data.media_id_string;

			var tweet = {
				status : '   ',
				media_ids: [id]
			}
			T.post('statuses/update', tweet, tweeted);
		}
	}

	function tweeted(err, data, response) {
		if (err){
			console.log("post error");
		} else {
			console.log("post_complete");
			find_next();
		}
	}
}