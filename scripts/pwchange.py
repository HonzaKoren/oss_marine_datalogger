import random
import string

from django.contrib.auth.models import User
from common import app_config

# Generate new random admin password
letters = string.ascii_lowercase
new_pass = ''.join(random.choice(letters) for i in range(10))

# Write given password to a shared memory for config server
shm = app_config.cfg
shm.write('admin_pwd', new_pass)

# Set the password for admin user
usr = User.objects.get(username='admin')
usr.set_password(new_pass)
usr.save()
