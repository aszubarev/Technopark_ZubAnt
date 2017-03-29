from django.test import TestCase
from cgi import parse_qs, escape


def say_hello(environ, start_response):
    status = '200 OK'
    response_headers = [('Content-type', 'text/plain')]
    start_response(status, response_headers)
    hello = b'Hello world!'
    return [hello]


html = """
<html>
<body>
   <form method="get" action="">
        <p>
           Name: <input type="text" name="name" value="%(name)s">
           Age: <input type="text" name="age" value="%(age)s">
        </p>
        <p>
            <input type="submit" value="Submit">
        </p>
    </form>
    <form method="post" action="">
        <p>
           Group: <input type="text" name="group" value="%(group)s">
           Course: <input type="text" name="course" value="%(course)s">
        </p>
        <p>
            <input type="submit" value="Submit">
        </p>
    </form>
    <p>
        Get-parameters:<br>
        Name: %(name)s<br>
        Age: %(age)s<br>
        Post-parameters:<br>
        Group: %(group)s<br>
        Course: %(course)s<br>
    </p>
</body>
</html>
"""


def application(environ, start_response):
    # the environment variable CONTENT_LENGTH may be empty or missing
    try:
        request_body_size = int(environ.get('CONTENT_LENGTH', 0))
    except (ValueError):
        request_body_size = 0

    # When the method is POST the variable will be sent
    # in the HTTP request body which is passed by the WSGI server
    # in the file like wsgi.input environment variable.
    request_body = environ['wsgi.input'].read(request_body_size)
    d_p = parse_qs(request_body)

    group = d_p.get('group', [''])[0]  # Returns the first group value.
    course = d_p.get('course', [''])[0]  # Returns the first course value.

    # Returns a dictionary in which the values are lists
    d_g = parse_qs(environ['QUERY_STRING'])

    # As there can be more than one value for a variable then
    # a list is provided as a default value.
    name = d_g.get('name', [''])[0]  # Returns the first name value
    age = d_g.get('age', [''])[0]  # Returns the first age value

    # Always escape user input to avoid script injection
    group = escape(group)
    course = escape(course)
    name = escape(name)
    age = escape(age)

    response_body = html % {  # Fill the above html template in
        'name': name or 'Empty',
        'age': age or 'Empty',
        'group': group or 'Empty',
        'course': course or 'Empty'
    }

    status = '200 OK'

    # Now content type is text/html
    response_headers = [
        ('Content-Type', 'text/html'),
        ('Content-Length', str(len(response_body)))
    ]

    start_response(status, response_headers)
    return [bytes(response_body.encode('utf-8'))]
