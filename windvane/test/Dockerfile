FROM python:3-alpine

COPY windvane.perftest.py /opt/
COPY test.sh /opt/

RUN chmod +x /opt/test.sh

WORKDIR /opt

RUN pip install requests

CMD /opt/test.sh

