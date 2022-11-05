# access_log off;
listen  443 ssl http2;
listen  [::]:443 ssl http2;
ssl_certificate /etc/nginx/ssl/selfsigned/selfsigned.crt;
ssl_certificate_key /etc/nginx/ssl/selfsigned/selfsigned.key;

error_log /var/log/nginx/cwcm_error.log;
access_log /var/log/nginx/scripts.log scripts;

set $bootstrap "index.php";
error_page 404 = /$bootstrap;

root /var/www/html/cwcmtest/docroot;

add_header X-Frame-Options "SAMEORIGIN";
add_header X-Xss-Protection "1; mode=block";
add_header Content-Security-Policy "frame-ancestors 'self'";
add_header X-Content-Type-Options "nosniff";
add_header referrer-policy "no-referrer-when-downgrade";

location ~ [^/]\.php(/|$) {
    include fastcgi_params;
    fastcgi_split_path_info ^(.+?\.php)(/.*)$;        
    set $orig_path $fastcgi_path_info;
    fastcgi_index $bootstrap;
    fastcgi_param PATH_INFO $orig_path;
    fastcgi_param SCRIPT_NAME $fastcgi_script_name;
    fastcgi_param SCRIPT_FILENAME $realpath_root$fastcgi_script_name;
    fastcgi_pass cwcmtest;
    try_files $uri =404;
}

location / {
    index $bootstrap;
    fastcgi_index $bootstrap;
    
}