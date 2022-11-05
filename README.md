# How to configure multiple PHP-FPM
## configure php-fpm
 - create docker-compose.yml
 - service name must be unique e.g. `cwcm` service name need to registed as server in nginx.conf file under http block, under upstream directive.
 - check the volume path attached here, with respect to server path. it must be sync with nginx server block root directive e.g. `/var/www/html/cwcm` this folder must be sync with document root in nginx server block configuration. This will be mislead to php files that nginx serving
 - `expose` port not requied here as php-fpm will be serving to 9000 port
 - `network` must be same with nginx network. otherwise it won't be able to communicate if they are on different network
 - Do not provide `container_name`, as when we have to scale docker container it need to create unique container name, this will difficult when you provide container name. When we do not provide it. it takes defualt project directory name, service name and then number of container. e.g. `phpfpm-cwcm-1`.
___

## configure nginx
 - create docker-compose.yml
 - make sure you are copy all `*.conf` files to the `/etc/nginx/conf.d/` location. eg. here `cwcm` and `cwcmtest` folder has `conf` files.
 - make sure volume mapping should be in sync with nginx server directive i.e document root path. e.g `cwcm` folder has document root folder `/var/www/html/cwcm/docroot` wheareas `cwcmtest` has document root `/var/www/html/cwcmtest/docroot`. make sure your php files coping to correct location.
 - same volume mapping goes to `log` folder also.
 - `network` must be sync and common for all `php-fpm` and `nginx`. so communication wont be happen. check with docker command
    - `docker network inspect <name>`. e.g. docker network inspect cex here as we are creating custom docker network with the name `cex`.
    - `docker network ls` e.g List all networks the engine daemon knows about, including those spanning multiple hosts
 - Nginx configuration settings
    - `http` directives in nginx.conf file must have multiple upsteam. e.g. `cwcm` and `cwcmtest` upstream. This upstream name must sync with server directives in `*.conf` files with location section `fastcgi_pass <upstream_name>;`. e.g `fastcgi_pass cwcm;` here and `fastcgi_pass cwcmtest;` in case of `cwcmtest` folder.
    - This upstream has container service name. which pointing to default php-fpm port. e.g `server cwcm:9000;`; here `cwcm` is container service name in `/home/vsavakhande/Projects/phpfpm1/docker-compose.yml` file.
    - `listen <port>` in server config should be expose in php-fpm. by defualt 443 and 80 port are expose in `php-fpm` docker image
 - Check which file is rending via 'nginx' configuration
    - check `log_format` in `http` directives in `nginx.conf`. eg. `log_format scripts '$document_root$fastcgi_script_name > $request';`. here `scripts` is custom log format.
    - this will get document root of script file being executed.
    - this must match with server directives `access_log /var/log/nginx/scripts.log scripts;` tag. here this `scripts` is pointing to `http` directives `log_format` tag.
 - Always check docker log for better understanding `docker container logs <container_name>`. eg. `docker container logs nginx`.

