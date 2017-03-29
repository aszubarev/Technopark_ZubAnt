def simple_app(environ, start_response):
    """Simplest possible application object"""
    status = '200 OK'

    output_message = [u'Hello']

    response_headers = [('Content-type', 'text/plain')]
    start_response(status, response_headers)

    return output_message

