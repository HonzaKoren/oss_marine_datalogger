from .base import *  # noqa
from .base import env

DEBUG = True

DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.sqlite3',
        'NAME': f'{ Path(BASE_DIR).resolve(strict=True).parent.parent.parent }/oss_ml_test/dev.sqlite3',
    }
}
