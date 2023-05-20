## Синтаксис программы

```

<Программа> ::= <Объявление переменных> <Описание вычислений>
<Описание вычислений> ::= Begin <Список операторов>  End
<Объявление переменных> ::= Var <Список переменных> ;
<Список переменных> ::= <Идент> | <Идент> , <Список переменных>
<Список операторов> ::= <Оператор> | <Оператор> <Список операторов>
<Оператор>::=<Присваивание> |<Сложный оператор> 
<Присваивание> ::= <Идент> := <Выражение> ;
<Выражение> ::= <Ун.оп.> <Подвыражение> | <Подвыражение>
<Подвыражение> :: = ( <Выражение> ) | <Операнд> |
< Подвыражение > <Бин.оп.> <Подвыражение>
<Ун.оп.> ::= "-"|"not"
<Бин.оп.> ::= "-" | "+" | "*" | "/"|"=<"|">="|"="
<Операнд> ::= <Идент> | <Const>
|<Сложный оператор>:: =<Оператор цикла> | <Составной оператор>
<Оператор цикла>:: =REPEAT <Список операторов>  UNTIL <Выражение>
<Составной оператор> ::= “{” <Список операторов> “}”
<Идент> ::= <Буква> <Идент> | <Буква>
<Const> ::= <Цифра> <Const> | <Цифра>


```

## Использование интерпретатора 

1) Создаем файл (или используем `test_program` из репозитория)
1) Пишем в него код программы на языке, описанном выше
1) Собираем интерпретатор командой `make`
1) `cat <your_file_name> | ./main`