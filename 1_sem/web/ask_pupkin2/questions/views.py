from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt


def append_body_answer(output_message):
    output_message.append('<html lang="en">'
                          '<head><meta charset="UTF-8"><title>Title</title></head>'
                          '<body>'
                          '<h1>GET</h1>'
                          '<form method="GET">'
                          '<input type="text" size="40" name="get_space" value="">'
                          '<input type="submit" value="Send">'
                          '</form>'
                          '<h1>POST</h1>'
                          '<form method="POST">'
                          '<input type="text" size="40" name="post_space" value="">'
                          '<input type="submit" value="Send">'
                          '</form>')


def append_end_body_answer(output_message):
    output_message.append('</body>'
                          '</html>')


def append_answer(output_message, method, query, answer):
    output_message.append('<h1>METHOD:</h1>')
    output_message.append(method)
    output_message.append('<h1>QUERY STRING:</h1>')
    output_message.append(query)
    output_message.append('<h1>Answer:</h1>')
    output_message.append(answer)


@csrf_exempt
def getpost(request):

    output_message = ['<!DOCTYPE html>']
    append_body_answer(output_message)

    if request.method == 'POST':
        ans = request.POST.get('post_space')
        if ans != '':
            append_answer(output_message, 'POST', ans, 'Hello_world')
        else:
            append_answer(output_message, 'POST', "empty input data", 'Hello_world')
        for k, v in request.POST.items():
            print(k, v)

    elif request.method == 'GET':
        ans = str()
        for key, value in request.GET.items():
            list_p = request.GET.getlist(key)
            for elem in list_p:
                keyvalue = key + " = " + elem + '\n'
                ans += keyvalue
        append_answer(output_message, 'GET', ans, 'Hello_world')

    append_end_body_answer(output_message)

    return HttpResponse(output_message)