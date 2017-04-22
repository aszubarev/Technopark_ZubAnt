from django.conf.urls import url
from django.contrib import admin
from questions.views import hot, login, ask, tag, question, signup

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^hot/', hot, name='hot'),
    url(r'^login/', login, name='login'),
    url(r'^ask/', ask, name='ask'),
    url(r'^signup/', signup, name='signup'),
    url(r'^(?P<question_id>[0-9]+)/$', question, name='question'),
    url(r'^tag/', tag, name='tag'),
]
