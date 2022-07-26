from django.contrib import admin
from .models import Configuration

# Register your models here.
@admin.register(Configuration)
class ConfigurationAdmin(admin.ModelAdmin):
    list_display = ('item', 'value')
    search_fields = ('name', 'comment')
    
