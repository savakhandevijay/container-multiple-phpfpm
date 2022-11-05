# How to configure multiple PHP-FPM
## Configure PHP-FPM
 - Create docker-compose.yml
 - Service name must be unique e.g. `cwcm` service name need to registed as server in nginx.conf file under http block, under upstream directive.
 - Check the volume path attached here, with respect to server path. it must be sync with nginx server block root directive e.g. `/var/www/html/cwcm` this folder must be sync with document root in nginx server block configuration. This will be mislead to php files that nginx serving
 - `expose` port not requied here as php-fpm will be serving to 9000 port
 - `network` must be same with nginx network. otherwise it won't be able to communicate if they are on different network
 - Do not provide `container_name`, as when we have to scale docker container it need to create unique container name, this will difficult when you provide container name. When we do not provide it. it takes defualt project directory name, service name and then number of container. e.g. `phpfpm-cwcm-1`.
___

## configure nginx
 - Create docker-compose.yml
 - Make sure you are copy all `*.conf` files to the `/etc/nginx/conf.d/` location. eg. here `cwcm` and `cwcmtest` folder has `conf` files.
 - Make sure volume mapping should be in sync with nginx server directive i.e document root path. e.g `cwcm` folder has document root folder `/var/www/html/cwcm/docroot` wheareas `cwcmtest` has document root `/var/www/html/cwcmtest/docroot`. make sure your php files coping to correct location.
 - Volume mapping goes to `log` folder also.
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

## Important Commands
 -  Build Container First Time `docker-compose up -d --build`
 -  Stop and Remove contrainers `docker-compose down --remove-orphans`
 -  Just bring container up `docker-compose up -d`
 -  List of active containers `docker ps`
 -  Connect/Login to Container `docker exec -it <contrainer_id> <shell_scripts>`. e.g `docker exec -it nginx sh`
 -  List of Process running inside container `docker top <container_id>`
 -  Stop only One container `docker-compose rm -s -v cron`
 -  ### Docker cleanup dangline/Unused images
   -  `docker system prune -a`
   -  `docker rm $(docker ps -q -f 'status=exited')`
   -  `docker rmi $(docker images -q -f "dangling=true")`
 -  Pass custom environment file while building docker image `docker-compose --env-file ./.env.uat up -d --build`
 -  To Kill all docker containers when there is error and no option to stop it. docker rm `docker ps --no-trunc -aq`
 -  for more commands follow 
   - [docker](https://docs.docker.com/engine/reference/commandline/docker)
   - [oh-my-zsh-docker](https://github.com/ohmyzsh/ohmyzsh/tree/master/plugins/docker)
   - [oh-my-zsh-docker-compose](https://github.com/ohmyzsh/ohmyzsh/tree/master/plugins/docker-compose)