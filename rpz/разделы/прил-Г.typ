#import "@local/gost732-2017:0.3.0": *
#import "@local/bmstu:0.2.0": *

#show: приложение.with(буква: "Г", содержание: [ Диаграммы временные ])

#страница(
  image("../схемы/временная.jpg", width: 100%, fit: "cover"),
  повернуто: да,
  формат: "a3",
  номер: нет
)