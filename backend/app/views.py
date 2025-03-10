"""
Flask Documentation:     https://flask.palletsprojects.com/
Jinja2 Documentation:    https://jinja.palletsprojects.com/
Werkzeug Documentation:  https://werkzeug.palletsprojects.com/
This file creates your application.
"""

# from crypt import methods
import site 

from app import app, Config,  mongo, Mqtt
from flask import escape, render_template, request, jsonify, send_file, redirect, make_response, send_from_directory 
from json import dumps, loads 
from werkzeug.utils import secure_filename
from datetime import datetime,timedelta, timezone
from os import getcwd
from os.path import join, exists
from time import time, ctime
from math import floor
 



#####################################
#   Routing for your application    #
#####################################


# 1. CREATE ROUTE FOR '/api/set/combination'
@app.route('/api/set/combination', methods=['POST'])
def set_combination():
    '''Sets combination'''

    if request.method == "POST":
        passcode = request.form.get('passcode')
        print(passcode)
        if passcode and len(passcode) == 4:
            result = mongo.update_passcode(passcode)
            if result:
                return jsonify({"status": "complete", "data": "complete"})
        return jsonify({"status": "failed", "data": "failed"}), 400
# 2. CREATE ROUTE FOR '/api/check/combination'

@app.route('/api/check/combination', methods=['POST'])
def check_combination():
    '''Checks combination'''

    if request.method == "POST":
        data = request.form.get("passcode")
        print("Huh")
        # data = request.json
        print(data)
        # # if 
        result = mongo.check_passcode(data)

        if result == 1:
            return jsonify({"status": "complete", "data": "complete"}) , 200
        return jsonify({"status": "failed", "data": "failed"}), 400

# 3. CREATE ROUTE FOR '/api/update'
# @app.route('/api/update', method=['POST'])
# def update_combination():
#     '''Returns combination'''

@app.route('/api/update', methods=['POST'])
def update_combination():
    '''Updates combination'''

    if request.method == "POST":
        data = request.form.get("passcode")
        print("In Update")
        # data = request.json
        print(data)
        # # if 
        result = mongo.update_passcode(data)

        if result:
            return jsonify({"status": "complete", "data": "complete"}) , 200
        return jsonify({"status": "failed", "data": "failed"}), 400

@app.route('/api/update',methods=["POST"])
def update():
    if request.method=='POST':
        data = request.get_json()
        timestamp = datetime.utcnow().timestamp()
        data['timestamp'] = timestamp
        Mqtt.publish('620155671',json.dumps(data))
        result = mongo.update(data)
        if result.inserted_id:
            return jsonify({"status":"complete","data":"complete"})
        else:
            return jsonify({"status":"failed","data":"failed"})



@app.route('/api/reserve/<start>/<end>',methods=['GET'])
def reserve(start,end):
    start_t  = int(start)
    end_t = int(end)
    if request.method=="GET":
        result = mongo.get_reserve(start_t,end_t)
        data = list(result)
        print(data)
        if data!=[]:
            return jsonify({"status":"found","data":data})
        return jsonify({"status":"failed",'data':0})




@app.route('/api/avg/<start>/<end>',methods=['GET'])
def avg(start,end):
    if request.method=='GET':
        result = mongo.get_avg(int(start),int(end))
        data = list(result)
        if result!='Failed':
            return jsonify({"status":"found","data":data[0].get('average')})
        return jsonify({"status":"failed","data":0})

# @app.route('/api/test')
# def send():
#     return jsonify({"status":"success"})
   
# 4. CREATE ROUTE FOR '/api/reserve/<start>/<end>'

# 5. CREATE ROUTE FOR '/api/avg/<start>/<end>'


   






@app.route('/api/file/get/<filename>', methods=['GET']) 
def get_images(filename):   
    '''Returns requested file from uploads folder'''
   
    if request.method == "GET":
        directory   = join( getcwd(), Config.UPLOADS_FOLDER) 
        filePath    = join( getcwd(), Config.UPLOADS_FOLDER, filename) 

        # RETURN FILE IF IT EXISTS IN FOLDER
        if exists(filePath):        
            return send_from_directory(directory, filename)
        
        # FILE DOES NOT EXIST
        return jsonify({"status":"file not found"}), 404


@app.route('/api/file/upload',methods=["POST"])  
def upload():
    '''Saves a file to the uploads folder'''
    
    if request.method == "POST": 
        file     = request.files['file']
        filename = secure_filename(file.filename)
        file.save(join(getcwd(),Config.UPLOADS_FOLDER , filename))
        return jsonify({"status":"File upload successful", "filename":f"{filename}" })

 


###############################################################
# The functions below should be applicable to all Flask apps. #
###############################################################


@app.after_request
def add_header(response):
    """
    Add headers to both force latest IE rendering engine or Chrome Frame,
    and also tell the browser not to cache the rendered page. If we wanted
    to we could change max-age to 600 seconds which would be 10 minutes.
    """
    response.headers['X-UA-Compatible'] = 'IE=Edge,chrome=1'
    response.headers['Cache-Control'] = 'public, max-age=0'
    return response

@app.errorhandler(405)
def page_not_found(error):
    """Custom 404 page."""    
    return jsonify({"status": 404}), 404



