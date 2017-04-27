from django.db import models
from django.contrib.auth.models import User
from django.db.models import Count
from django.http import Http404


class QuestionsManager(models.Manager):
    def get_by_id(self, q_id):
        try:
            return self.get(pk=q_id)
        except Questions.DoesNotExist:
            raise Http404('Question not found')


class AnswersManager(models.Manager):
    pass


class TagsManager(models.Manager):
    def get_tags_for_question_by_id(self, q_id):
        return self.filter(question=q_id)

    def get_popular_tags(self):
        return self.annotate(Count('question')).order_by('-question__count')[:10]

    def get_tag_by_name(self, tag_name):
        return self.get(name=tag_name)


class ProfileManager(models.Manager):
    def get_profile_by_user_id(self, user_id):
        return self.get(user=user_id)


class Questions(models.Model):
    profile = models.ForeignKey('Profile', on_delete=models.SET_NULL, blank=True, null=True)
    title = models.CharField(max_length=255)
    text = models.TextField()
    rating = models.IntegerField(default=0)
    pub_date = models.DateTimeField(auto_now_add=True)
    tags = models.ManyToManyField('Tags')

    objects = QuestionsManager()

    class Meta:
        db_table = 'Questions'


class Answers(models.Model):
    profile = models.ForeignKey('Profile', on_delete=models.SET_NULL, blank=True, null=True)
    question = models.ForeignKey('Questions', on_delete=models.CASCADE, blank=False, null=False)
    text = models.TextField()
    rating = models.IntegerField(default=0)
    pub_date = models.DateTimeField(auto_now_add=True)

    objects = AnswersManager()

    class Meta:
        db_table = 'Answers'


class Tags(models.Model):
    name = models.CharField(max_length=255)
    objects = TagsManager()

    class Meta:
        db_table = 'Tags'


class Profile(models.Model):
    user = models.OneToOneField(User, on_delete=models.CASCADE)
    objects = ProfileManager()

    class Meta:
        db_table = 'Profile'
