from flask import Flask, request
import process

data = process.Reader('rainfall.csv')

app = Flask(__name__)

if __name__ == '__main__':
    app.run(port = 80)


@app.route("/")

def station_find():

    station = request.args.get('stationReference',type = str)
    
    station_data = data.data[data.data.stationReference == station]
    
    station_data = station_data[['dateTime','value']]

    station_data = station_data.sort_values(by = 'dateTime')
    
    table_station = station_data.to_html(index = False)

    

    return table_station