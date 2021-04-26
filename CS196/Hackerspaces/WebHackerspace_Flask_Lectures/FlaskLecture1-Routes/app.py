from flask import Flask
from flask import render_template
app = Flask(__name__)

@app.route('/')
def index():
	return "<h1> Hello World! </h1>"

@app.route('/user/<name>')
def user(name):
	return "<h1> Hello %s!</h1>" % name

@app.route('/user2/<name>')
def user2(name):
     return render_template('index.html', name=name)

if __name__ == '__main__':
	consumer_key = "Qv02gUSNtlBuSGCn3uCYCUXnV"
    consumer_secret = "Ids1UfKrrAXwB2iSbU3TwvLBzgrK4VWw9tpkWnLYtSoo56RbyX"
    access_token = "783152971878047745-OicwurGvGxOelZuGMnttbOzkbfiYEgX"
    acess_token_secret = "DyUfQf3jp4JGHyhuoWDuJkKold2YVrTs9B87QfR4KoxKs"
    twitter = Twitter( auth=OAuth(access_token, acess_token_secret, consumer_key, consumer_secret) )
	query = "#" + input
    my_tweets = twitter.search.tweets(q=query)
    hashtags = "RELATED HASHTAGS: " 
    if (len(my_tweets['statuses']) != 0):
        for i in range(len(my_tweets['statuses'][0]['entities']['hashtags'])):
            hashtags += my_tweets['statuses'][0]['entities']['hashtags'][i]['text'] + "  "
	app.run(debug=True)
