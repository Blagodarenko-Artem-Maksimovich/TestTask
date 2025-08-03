# TestTask Unreal Engine Project (UE 5.3.2)

## Архитектура (MVC)

| Компонент         | Классы и роли             | Отвечает за...                    |
|-------------------|---------------------------|-----------------------------------|
| **Model (данные)**   | `FObjectData`, `FSaveObjectsContainer` | Хранит состояние объектов;       |
|                   |                           | сериализуется/десериализуется из JSON (JsonObjectConverter) |
| **View (UI + Scene)** | `AInteractiveObject`       | Визуализация: рендер меша + динамический материал `MI_Dynamic`; |
|                   | `UObjectListWidget`        | Показ списка объектов и их статусов: цвет, имя, активность |
| **Controller (логика)** | `AGS_GameStateBase`         | Загружает JSON в начале, спаунит объекты, хранит их данные (модель); |
|                   |                           | отправляет изменения в UI через делегаты (`OnObjectsUpdated`); |
|                   | `AWD_HUD` или `AHUDBase`   | Создаёт UI и работать с ним; |
|                   | `ATestTaskCharacter`       | Обрабатывает ввод игрока, выполняет `Interact()` |

### Как взаимодействуют компоненты

1. **Начальный загруз JSON → `AGS_GameStateBase::LoadObjectsFromJson()`**:  
   Десериализует JSON в `FSaveObjectsContainer`, очищает массив, передаёт каждому объекту данные -> вызывает `AInteractiveObject::InitFromData()` для спауна.

2. **Интеракт**:  
   `ATestTaskCharacter::Interact()` отправляет сигнал `Interact_Implementation()` на `AInteractiveObject`, который обновляет `ObjectData.bIsActive` и цвет через `MI_Dynamic`, затем вызывает `GameState->UpdateObjectData()`.

3. **Обновление UI**:  
   GameState в `UpdateObjectData()` обновляет данные в массиве и отправляет `OnObjectsUpdated` делегат. `UObjectListWidget` подписан через `AddUniqueDynamic`; его `RefreshList()` отображает актуальный список.

4. **Завершение сессии и сохранение**:  
   При нажатии кнопки «Завершить сессию» в виджете вызывается `SaveObjectsToJson()` -> JSON форматируется и сохраняется в `Saved/SessionSave/SessionSave.json`. Затем вызывается `QuitGame()` по желанию пользователя.

Это соответствует классическим принципам MVC, где Model информирует View об изменениях через Controller, а View реагирует мгновенно на обновления состояния.  :contentReference[oaicite:0]{index=0}

---
##  Что реализовано

**Библиотека JsonUtilities** (`FJsonObjectConverter`)  
  сериализация и десериализация `USTRUCT` с JSON в удобном формате, без сторонних libs.  :contentReference[oaicite:1]{index=1}

- Мутси **динамические материалы** (`UMaterialInstanceDynamic`)  
  Выбор цвета по `ColorName` (`FLinearColor`) через параметр `"ColorParam"` позволяет менять цвет объектов на лету без дублирования материалов.  :contentReference[oaicite:2]{index=2}

**Универсальный механизм загрузки объектов**  
  Имена мешей извлекаются по `NameToMeshMap`. Можно добавить новые объекты просто в JSON (без изменений в C++):  
  `{ "name": "Cube42", ... }` → отобразится как куб благодаря совпадению имени. Загрузка новых цветов аналогично.  

**Чистый и модульный дизайн кода на C++**  
  *Контроллер* (`AGS_GameStateBase`) отделён от *интерфейса* (`UObjectListWidget`), а *представление* (`AInteractiveObject`) — от *модели* (`FObjectData`).  
  Каждая ответственность (UI, логика, данные) изолирована и может изменяться независимо.

**Обработка ошибок**  
  методы `ensure(...)`, логи `UE_LOG(...)`, проверки nullptr, fallback-меш `Cube` и дефолтный цвет при отсутствии ключа.

**Механизм сохранения и выхода**  
  При нажатии «Завершить сессию» данные сохраняются и затем вызывается `UKismetSystemLibrary::QuitGame()`, что завершает игру корректно.

Поддержка **различных JSON-состояний**, включая новые объекты и цвета, даже с пустым состоянием (UI отобразит пустой список).

---

##  Файловая структура

Source/
├─ SaveObjectsContainer.h 
├─ ObjectData.h
├─ InteractiveObject.h/.cpp
├─ GS_GameStateBase.h/.cpp
├─ HUDBase.h/.cpp
├─ ObjectListWidget.h/.cpp
├─ InteractableInterface.h
... Blueprint classes:
WB_ObjectList, WI_WBPRow, MI_Dinamic, ...
Saved/
└─ SessionSave/
└─ SessionSave.json

---

## Setup & Запуск

1. Поместите `Save.json` с массивом `[ { id, name, position, colorName, isActive } ]`
2. Смена `position` на `{ "X":…,"Y":…,"Z":… }`, ключ `colorName` должен совпадать с соответствующим `NameToColorMap`
3. В редакторе: Установите `GameState` в `World Settings`, не забудьте указать `HUDBase` в `GameMode`
4. Задайте widget blueprint `WB_ObjectList` как потомок `UObjectListWidget`, привяжите `ListBox` и `FinishSessionButton`
5. Постройте билд — игра загрузится, при клике по объектам их цвет и активность изменяются, UI обновляется, файл сохранится в `Saved/SessionSave/SessionSave.json`

---

## Особенности внедрения MVC в UE

- `AGS_GameStateBase` как **Controller + части Model** — интерфейс для загрузки, обновлений, подписки.
- `FObjectData` — **Model**, чистая структура данных, не привязанная к группе классов.
- `UObjectListWidget` / `InteractiveObject` — **View**, реагируют на изменения модели и перерисовываются.
- Делегат `OnObjectsUpdated` — реализация **Observer pattern**, так Model notifies View.  :contentReference[oaicite:3]{index=3}

