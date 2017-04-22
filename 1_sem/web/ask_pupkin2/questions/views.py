from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage


questions = []
for i in range(1, 5):
    questions.append({'title': 'title ' + str(i),
                      'id': i,
                      'text': 'text' + str(i)
                      })


def main_func(request):
    return render(request, 'main.html')


def hot(request):
    page = paginate(questions, request)
    return render(request, 'hot.html', {'page': page})


def tag(request):
    return render(request, 'tag.html')


def question(request, question_id):
    return render(request, 'question.html', {'question_id': question_id})


def login(request):
    return render(request, 'login.html')


def signup(request):
    return render(request, 'register.html')


def settings(request):
    return render(request, 'settings.html')


def ask(request):
    return render(request, 'ask.html')


def paginate(list, request):
    paginator = Paginator(list, 3)
    pageNum = request.GET.get('page')
    try:
        page = paginator.page(pageNum)
        if pageNum != None:
            pageNum = int(pageNum)
    except PageNotAnInteger:
        page = paginator.page(1)
        pageNum = 1
    except EmptyPage:
        page = paginator.page(paginator.num_pages)
        pageNum = paginator.num_pages
    page.page_range_current = get_page_range(pageNum, paginator)
    return page


def get_page_range(current, paginator):
    count = 5
    wing = 2
    if current - wing > 0:
        if current + wing <= paginator.num_pages:
            page_range = range(current - wing, current + wing + 1)
        else:
            page_range = range(current - (count - (paginator.num_pages + 1 - current)), paginator.num_pages + 1)
    else:
        if current + count - 1 <= paginator.num_pages:
            page_range = range(1, count + 1)
        else:
            page_range = range(1, paginator.num_pages + 1)
    return page_range

