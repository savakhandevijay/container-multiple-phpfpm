FROM nginx:stable-alpine

COPY ./nginx/conf.d/cwcm/* /etc/nginx/conf.d/
COPY ./nginx/conf.d/cwcmtest/* /etc/nginx/conf.d/
COPY ./nginx/greetings.sh .

RUN chmod +x /greetings.sh

COPY ./nginx/*.conf /etc/nginx/

RUN mkdir -p /etc/nginx/ssl

COPY ./nginx/ssl /etc/nginx/ssl