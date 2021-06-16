""" Module containing a class to process rainfall data."""
import pandas as pd
import numpy as np
from urllib.request import urlopen
import json


class Reader:
    """Class to process rainfall data."""

    def __init__(self, filename):
        """Read in the rainfall data from a named .csv file using pandas

        The data is stored in a class variable reader.data

        Parameters
        ----------

        filename: str
            The file to be read

        >>> Reader('rainfall.csv').data.loc[0].stationReference
        'E12560'
        """
        self.data = pd.read_csv(filename)
        
        pd.to_numeric(self.data.value,errors = 'coerce')

        self.data.value.fillna(0)

        
     

    def station_total_rainfall(self, station_ref):
        """Return the total rainfall at station reference station_ref.

        Parameters
        ----------

        station_ref: str
            Station Reference

        Returns
        -------

        float
            Total rainfall

        >>> round(Reader('rainfall.csv').station_total_rainfall('E24767'), 1)
        2.6
        """

        station_rainfall_1 = self.data.value[self.data.stationReference == station_ref]
        
        total = station_rainfall_1.astype('float64').sum()

        return  total

    def station_rainfall(self, station_ref):
        """Return the rainfall at station reference station_ref
        as a pandas Series, indexed by the dateTime series.

        Parameters
        ----------

        station_ref: str
            Station Reference

        Returns
        -------

        pandas.Series
            Station rainfall values (indexed by date & time)

        >>> series = Reader('rainfall.csv').station_rainfall('E24767')
        >>> float(series.loc['2020-10-12T02:00:00Z'])
        0.2
        """
        station_rainfall_2 = self.data[self.data.stationReference == station_ref]
        
        old_index = station_rainfall_2.index

        new_index = station_rainfall_2.dateTime

        station_rainfall_alter = station_rainfall_2.value.rename(index = dict(zip(old_index,new_index)))





        return station_rainfall_alter

    def station_url(self, station_ref):
        """Return the API URL for station reference station_ref

        Parameters
        ----------

        station_ref: str
            Station Reference

        Returns
        -------

        str
            REST API URL
  

        >>> Reader('rainfall.csv').station_url('261923TP')
        'http://environment.data.gov.uk/flood-monitoring/id/stations/261923TP'
        """ 

        station = self.data.station[self.data.stationReference == station_ref].unique()[0]

        return station

    def station_location(self, station_ref):
        """Returns API reported easting and northing for station
        reference station_ref

        Parameters
        ----------

        station_ref: str
            Station Reference

        Returns
        -------

        tuple
            easting, northing pair

        >>> Reader('rainfall.csv').station_location('E24767')
        (623550, 274550)
        """
        url = self.station_url(station_ref)

        open_data = urlopen(url)

        data = open_data.read()

        new_dict = json.loads(data)

        tuple_location = (new_dict['items']['easting'],new_dict['items']['northing'])


        return tuple_location

