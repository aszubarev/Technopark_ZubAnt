from django.conf.urls import url
from django.contrib import admin
from questions.views import hot, login_view, ask, tag, question_view, signup

urlpatterns = [
    url(r'^admin/', admin.site.urls),
    url(r'^hot/', hot, name='hot'),
    url(r'^login/', login_view, name='login'),
    url(r'^ask/', ask, name='ask'),
    url(r'^signup/', signup, name='signup'),
    url(r'^(?P<question_id>[0-9]+)/$', question_view, name='question'),
    url(r'^tag/(?P<tag_id>[0-9]+)', tag, name='tag'),
]
