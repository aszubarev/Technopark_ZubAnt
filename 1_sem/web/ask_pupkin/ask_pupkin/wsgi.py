"""
WSGI config for ask_pupkin project.

It exposes the WSGI callable as a module-level variable named ``application``.

For more information on this file, see
https://docs.djangoproject.com/en/1.10/howto/deployment/wsgi/
"""


import os

from django.core.wsgi import get_wsgi_application

os.environ.setdefault("DJANGO_SETTINGS_MODULE", "ask_pupkin.settings")


def append_body_answer(output_message):
    output_message.append(b'<html lang="en">'
                          b'<head><meta charset="UTF-8"><title>Title</title></head>'
                          b'<body>'
                          b'<h1>GET</h1>'
                          b'<form method="GET">'
                          b'<input type="text" size="40" name="get_space" value="">'
                          b'<input type="submit" value="Send">'
                          b'</form>'
                          b'<h1>POST</h1>'
                          b'<form method="POST">'
                          b'<input type="text" size="40" name="post_space" value="">'
                          b'<input type="submit" value="Send">'
                          b'</form>')


def append_end_body_answer(output_message):
    output_message.append(b'</body>'
                          b'</html>')


def append_answer(output_message, method, query, answer):
    output_message.append(b'<h1>METHOD:</h1>')
    output_message.append(method)
    output_message.append(b'<h1>QUERY STRING:</h1>')
    output_message.append(query)
    output_message.append(b'<h1>Answer:</h1>')
    output_message.append(answer)


def simple_app(environ, start_response):
    """Simplest possible application object"""
    status = '200 OK'
    print("============================================================")
    for hui in environ:
        print(hui, environ[hui])

    output_message = [b'<!DOCTYPE html>']
    append_body_answer(output_message)

    if environ['REQUEST_METHOD'] == 'POST':
        query_string = environ['QUERY_STRING']
        print("query_string = ", query_string)

        body_post = environ['wsgi.input']
        body_byte_post = body_post.read()
        print("body_byte_post = ", body_byte_post)

        if body_byte_post != b'':
            append_answer(output_message, b'POST', body_byte_post, body_byte_post)
        else:
            append_answer(output_message, b'POST', b'Empty input',  b'Empty answer')

    if environ['REQUEST_METHOD'] == 'GET':
        query_string = environ['QUERY_STRING']
        print("query_string = ", query_string)

        body_get = environ['wsgi.input']
        body_byte_get = body_get.read()
        print('body_byte: ', body_byte_get)

        if query_string != '':
            append_answer(output_message, b'GET', bytes(query_string.encode()), b'Hello, my friend!!!')
        else:
            append_answer(output_message, b'GET', b'Empty query', b'Hello, my friend!!!')

    append_end_body_answer(output_message)

    response_headers = [('Content-type', 'text/html')]
    start_response(status, response_headers)

    return output_message


def say_hello(environ, start_response):
    status = '200 OK'
    response_headers = [('Content-type', 'text/plain')]
    start_response(status, response_headers)
    hello = b'Hello world!'
    return [hello]


class AppClass:
    """Produce the same output, but using a class

    (Note: 'AppClass' is the "application" here, so calling it
    returns an instance of 'AppClass', which is then the iterable
    return value of the "application callable" as required by
    the spec.

    If we wanted to use *instances* of 'AppClass' as application
    objects instead, we would have to implement a '__call__'
    method, which would be invoked to execute the application,
    and we would need to create an instance for use by the
    server or gateway.
    """

    def __init__(self, environ, start_response):
        self.environ = environ
        self.start = start_response

    def __iter__(self):
        status = '200 OK'
        response_headers = [('Content-type', 'text/plain')]
        self.start(status, response_headers)
        yield "Hello world!\n"

application = get_wsgi_application()
