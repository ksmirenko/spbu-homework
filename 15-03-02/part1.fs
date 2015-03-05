// Задачи 9-13 от 2 марта
// Автор: Кирилл Смиренко, группа 171
// Расчётное время выполнения: 2 часа
// Действительное время выполнения: 3 часа

// задание  9 (тип List.iter)
// val it : (('a -> unit) -> 'a list -> unit) = <fun:clo@2>

// задание 10 (реверс списка через fold)
let reverseList list =
  List.fold (fun list elem -> elem::list) [] list

// задание 11 (filter через fold)
let filterList f list =
  List.foldBack (fun elem list -> if (f elem) then elem::list else list) list []
  // Я решил, что foldBack - это почти fold, только работать это будет быстрее

// задание 12 (map через fold)
let mapList f l =
  List.foldBack (fun elem l -> (f elem)::l) l []

// задание 13 (схема Горнера вычисления значения многочлена в т. x0)
// коэфф-ты передаются в формате [aN .. a0]
let horner a x0 =
  List.fold (fun acc elem -> elem + acc * x0) 0 a

[<EntryPoint>]
let main argv =
  let l = [1 .. 10]
  printf "Тестовый список для всех заданий: "
  printfn "%A\n" l

  printf "Реверс списка: "
  printfn "%A\n" (reverseList l)

  printf "Фильтрация списка: "
  printfn "%A\n" (filterList (fun a -> (a % 3 = 0)) l)

  printf "Прибавление к числам из списка 5: "
  printfn "%A\n" (mapList (fun a -> a + 5) l)

  let a = [2; -3; 1; -11]
  let x0 = 6
  printfn "Схема Горнера\nКоэффициенты: %A\nx0 = %A\nОтвет = %A\n" a x0 (horner a x0)

  0