@startuml
activate master
master -> master: main
activate master
master -> master:ngx_save_argv
Bob --> Alice: Authentication Response

Alice -> Bob: Another authentication Request
Alice <-- Bob: Another authentication Response
@enduml