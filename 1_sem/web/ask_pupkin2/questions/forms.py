from django import forms
from questions.models import Profile, Questions, Tags
from django.contrib.auth.models import User


class RegisterForm(forms.Form):
    login = forms.CharField(label='Login', max_length=255, min_length=4)
    password = forms.CharField(label='Password', widget=forms.PasswordInput(), max_length=255, min_length=8)
    email = forms.EmailField(max_length=100)
    repeat_password = forms.CharField(widget=forms.PasswordInput(), max_length=100, min_length=8)

    def create_user(self):
        user = User.objects.create_user(self.cleaned_data['login'], self.cleaned_data['email'],
                                        self.cleaned_data['password'])
        user.save()
        profile = Profile.objects.create(user=user)
        profile.save()

    def clean(self):
        data = self.cleaned_data
        if data['password'] != data['repeat_password']:
            msg = 'Passwords not equals'
            self.add_error('password', msg)
            self.add_error('repeat_password', msg)

    def save(self):
        self.create_user()


class LoginForm(forms.Form):
    login = forms.CharField(label='Login', max_length=255, min_length=4)
    password = forms.CharField(label='Password', widget=forms.PasswordInput(), max_length=255, min_length=8)


class AskForm(forms.Form):

    title = forms.CharField(label='Title', max_length=255,
                            widget=forms.TextInput(
                                attrs={'class': 'form-control custom-input-text',
                                       'placeholder': 'Enter title'
                                       }))
    text = forms.CharField(label='Text', max_length=255,
                           widget=forms.Textarea
                           (attrs={'class': 'form-control custom-textarea-text',
                                   'placeholder': 'Enter text',
                                   'rows': '10'
                                   }))
    tags = forms.CharField(label='Tags', max_length=40, error_messages={'required': 'Fill text'},
                           widget=forms.TextInput(attrs={
                               'class': 'form-control custom-input-text',
                               'placeholder': 'Enter tags with spaces'
                           }))

    def create_question(self, user):
        data = self.cleaned_data

        title = data.get('title')
        text = data.get('text')

        tags_str = data['tags']
        tags_list = tags_str.split()
        tags_objects_list = list()

        profile = Profile.objects.get(user=user)

        for tag in tags_list:
            tag_lower_name = tag.lower()
            try:
                tag_object = Tags.objects.get_tag_by_name(tag_lower_name)
                tags_objects_list.append(tag_object)

            except Tags.DoesNotExist:
                tag_object = Tags.objects.create(name=tag_lower_name)
                tag_object.save()
                tags_objects_list.append(tag_object)

        print("creating question")
        question = Questions.objects.create(profile=profile, title=title, text=text)
        for tag_object in tags_objects_list:
            question.tags.add(tag_object)

        return question

    def save(self, user):
        question = self.create_question(user)
        return question.pk





