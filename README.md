# Online Library Client

This is a HTTP client that work with a REST API that works with an online library

# Implementation

Information is sent and received as JSONs
Authentification : JWT based

Choosen library : parson.h
* Lightweight
* Full JSON support

Types of request used :
* GET
* POST
* DELETE

# Usage

Comands:
* exit
* register
* login
* enter_library
* get_books
* get_book
* add_book
* delete_book
* logout

## Usage Examples

Examples 1 : Getting a book
```
login
username: USERNAME
password: PASSWORD

enter_library
Successfull enter_library
get_books

1 Book1
2 Book2

get_book 1
Title		Book1
Author		The first writer
Page count	10
Genre		Lorem Ipsum
Publisher	Lorem Ipsum Editure
```

Exaple 2 : Deleting a book
```
delete_book 2
Successfull delete

delete_book 2
No book was deleted!

get_book 2
Error during get_book

```

Example 3 : Unauthorised library access
```
enter_library
Error during enter_library
You are not logged in
```
Example 4 : Unauthorised get_book/get_books
```
login
username: USERNAME
password: PASSWORD
get_books
Error during get_books
You have no access!
```

# Cookies

The client stores the cookies it got and the last JWT-token it has received

Commands and what errors they signal

![](./misc/err_table.png)

Commnads and their interaction with cookies and headers

![](./misc/cookie_table.png)
