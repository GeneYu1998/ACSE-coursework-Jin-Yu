# ACSE 1 Assessment 2

**Deadline: 4pm BST 23rd October 2020**

## Task description

In this exercise you will use Pandas and Docker to create a tool to process rainfall data in `.csv` format into a more readable form.

The `rainfall.csv` file contains readings for rainfall (in mm) recorded over 15 minute windows at tipping bucket rainfall gauges around England and Wales for Monday 12th October 2020. The columns are:

- `dateTime`: The date and time of the 15 minute rainfall window for which the reading is valid.
- `station`: The URL for the station data in the UK Environment Agency API.
- `stationReference`: The short station reference.
- `value`: the rainfall reading in mmm.

### Workflow

You are strongly recommended to work using a code editor such as VS Code, rather than working in Jupyter notebooks and copying material back and forth. The entire repository should be uploaded to GitHub before the submission deadline. For this we recommend using either:
1. A `git` based GUI such as GitHub Desktop
2. The source control tab in VS Code
3. The `git` tool on the command line. If no additional files have been created, then this can be achieved by running:
  ```
  git add -u
  git commit
  git push -u origin main
  ```
  If you have created additional files to be submitted (e.g. additional code tests) then these should be staged using commands such as `git add test_process.py` before committing. 

### Problem Specification

You must:

1. [50 marks] Write appropriate Python code to complete the methods of class `Reader` in `process.py`. See the docstrings in each method for details of key functionality required, although other methods or functions can be added if appropriate.
2. [25 marks] Write Python code in `app.py` to serve an API such that a call to the endpoint `/?stationReference=E24767` returns output equivalent to the HTML table shown below the next task. You may either use Flask or builtin Python modules.
3. [25 marks] Write a Dockerfile which runs your `app.py` on the container's port 80, so that the command sequence
  ```
  docker build --tag rainfall:latest
  docker run -it -p5000:80 rainfall:latest
```
  allows a user to connect to `http://localhost:5000/?stationReference=E24767` and see an HTML table equivalent to the following:

<table border="1" class="dataframe">
  <thead>
    <tr style="text-align: right;">
      <th></th>
      <th>value</th>
    </tr>
    <tr>
      <th>dateTime</th>
      <th></th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th>2020-10-12T00:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T00:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T00:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T00:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T01:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T01:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T01:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T01:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T02:00:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T02:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T02:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T02:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T03:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T03:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T03:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T03:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T04:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T04:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T04:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T04:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T05:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T05:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T05:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T05:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T06:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T06:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T06:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T06:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T07:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T07:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T07:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T07:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T08:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T08:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T08:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T08:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T09:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T09:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T09:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T09:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T10:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T10:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T10:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T10:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T11:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T11:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T11:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T11:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T12:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T12:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T12:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T12:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T13:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T13:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T13:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T13:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T14:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T14:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T14:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T14:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T15:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T15:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T15:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T15:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T16:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T16:15:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T16:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T16:45:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T17:00:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T17:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T17:30:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T17:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T18:00:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T18:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T18:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T18:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T19:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T19:15:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T19:30:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T19:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T20:00:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T20:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T20:30:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T20:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T21:00:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T21:15:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T21:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T21:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T22:00:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T22:15:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T22:30:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T22:45:00Z</th>
      <td>0.000</td>
    </tr>
    <tr>
      <th>2020-10-12T23:00:00Z</th>
      <td>0.200</td>
    </tr>
    <tr>
      <th>2020-10-12T23:15:00Z</th>
      <td>0.0</td>
    </tr>
    <tr>
      <th>2020-10-12T23:30:00Z</th>
      <td>0.0</td>
    </tr>
    <tr>
      <th>2020-10-12T23:45:00Z</th>
      <td>0.0</td>
    </tr>
  </tbody>
</table>

### Mark scheme

Marks will be awarded for each part of the task completed, independent of the other parts of the task. The primary source of marks will be functionality based, with robustness and maintainability key consideration, although Python code should also be PEP8 compliant, Marks will not be lost for time or memory inefficiency, unless particularly egregious.





