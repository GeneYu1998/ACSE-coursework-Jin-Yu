# set base image to build on
FROM python:3.8

# set/create current working directory inside container
WORKDIR /rainfall

# copy all necessary files from the host to the container
COPY requirements.txt .

COPY app.py .

COPY process.py .

COPY rainfall.csv .


# install python modules
RUN pip install -r requirements.txt

# run app.py when container starts
CMD python app.py