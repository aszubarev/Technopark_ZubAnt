from django.shortcuts import render, redirect, resolve_url
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from django.core.paginator import Paginator, PageNotAnInteger, EmptyPage
from questions.forms import RegisterForm, LoginForm, AskForm
from django.contrib.auth.decorators import login_required
from django.contrib.auth import authenticate, login

from questions.models import Questions
questions_pag = []
for i in range(1, 5):
    questions_pag.append({'title': 'title ' + str(i),
                      'id': i,
                      'text': 'text' + str(i)
                      })


def signup(request):

    form = RegisterForm(request.POST or None)
    if request.method == "POST":
        if form.is_valid():
            form.save()
            return redirect(resolve_url('hot'))
        else:
            print("RegisterForm not valid")

    return render(request, 'register.html', {'form': form})


def hot(request):
    page = paginate(questions_pag, request)
    return render(request, 'hot.html', {'page': page})


def tag(request, tag_id):
    return render(request, 'tag.html')


def question_view(request, question_id):
    question = Questions.objects.get_by_id(question_id)
    return render(request, 'question.html', {'question': question, 'question_id': question_id})


def login_view(request):
    form = LoginForm(request.POST or None)

    if request.method == "POST":
        if form.is_valid():

            username = form.cleaned_data.get('login')
            password = form.cleaned_data.get('password')

            user = authenticate(username=username, password=password)
            if user is not None:
                login(request, user)
                url_continue = resolve_url('hot')
                return redirect(url_continue)

    return render(request, 'login.html', {'form': form, 'continue': resolve_url('login')})


def settings(request):
    return render(request, 'settings.html')


@login_required(login_url='/questions/login/')
def ask(request):
    form = AskForm(request.POST or None)

    if request.method == "POST":
        if form.is_valid():
            q_id = form.save(request.user)
            return redirect(resolve_url('question', question_id=q_id))

    return render(request, 'ask.html', {'form': form})


def paginate(list_data, request):
    paginator = Paginator(list_data, 3)
    page_num = request.GET.get('page')
    try:
        page = paginator.page(page_num)
        if page_num is not None:
            page_num = int(page_num)
    except PageNotAnInteger:
        page = paginator.page(1)
        page_num = 1
    except EmptyPage:
        page = paginator.page(paginator.num_pages)
        page_num = paginator.num_pages
    page.page_range_current = get_page_range(page_num, paginator)
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

