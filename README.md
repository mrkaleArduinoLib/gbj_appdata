<a id="library"></a>

# gbj\_appdata
This is an application library, which is used usually as a project library for particular PlatformIO project. It encapsulates the functionality of `central project data management`. The encapsulation provides following advantages:

* Functionality is hidden from the main sketch.
* The library follows the principle `separation of concerns`.
* The library is reusable for every project with its derived class acting as a central project data hub.
* Update in library is valid for all involved projects.


## Fundamental functionality
* The class from the library is not intended to be used directly in a sketch, just as a parent class for project specific data hub libraries, e.g., `app_data`.
* The class provide definition of a structure for a data item management with its methods and members, mostly flags.
* A data item value is kept in the library instance object always in string form in order to publish it independent from its original data type.


<a id="dependency"></a>

## Dependency
* **gbj\_serial\_debug**: Auxilliary library for debug serial output loaded from the file `gbj_serial_debug.h`. It enables to exclude serial outputs from final compilation.

#### Espressif ESP8266 platform
* **Arduino.h**: Main include file for the Arduino platform.

#### Espressif ESP32 platform
* **Arduino.h**: Main include file for the Arduino platform.

#### Particle platform
* **Particle.h**: Includes alternative (C++) data type definitions.


<a id="constants"></a>

## Constants
* **VERSION**: Name and semantic version of the library.

Other constants, enumerations, result codes, and error codes are inherited from the parent library.


<a id="interface"></a>

## Custom data types
* [Parameter](#parameter)

## Interface

* [gbj_appdata()](#gbj_appdata)
* [getPrmName()](#getPrmName)


<a id="parameter"></a>

## Parameter

#### Description
The structure with members and member methods as a template of a data item.
* A data item as an instance of this structure acts as a cache for entire project and is considered a part of the data hub.
* A set of boolean flags determines specific behaviour of an item, especially at definition as well as after publishing.
* While it is a structure, all methods are public even if some of them are used as private ones and should not be used outside of the definition.

#### Data item methods
* **Parameter(const char *key)**: Constructor for a data item.
  * **key**: The name of a data parameter. It is usually stored in flash memory for reducing operational memory usage and is generally prefixed as label with abbreviation `lbl`, e.g., _lblPeriodPublish_. Data item names are usually defined in include configuration files with names starting with `config_params` either in common or shared location, e.g., _config_params_gen.h_ or specifically in a project.
* **void set(_\<datatype\>_ value)**: The overloaded setter for updating a data item, each for particular data type. The method converts value from original data type to String data type and stores the provided value with help of the method `setValue`.
  * **value**: The new value of the data item. Its valid data types are defined by enumeration `Datatype` and can be:
    1. TYPE_NONE - Unknown data type is set at creating the data item or by method `reset`, and its string value is `n/a`.
    1. TYPE_BOOL - bool - Boolean data type with string values `true` and `false`.
    1. TYPE_BYTE - byte - Unsigned one character integer (8-bit) data type.
    1. TYPE_INT - int - Signed integer (16-bit) data type.
    1. TYPE_LONG - long - Signed long integer (32-bit) data type.
    1. TYPE_UINT - unsigned int - Unsigned integer (16-bit) data type.
    1. TYPE_LONG - unsigned long - Unsigned signed long integer (32-bit) data type.
    1. TYPE_FLOAT - float - Floating point numeric data type.
    1. TYPE_STRING - String - Stringified data type.
    1. TYPE_CCHAR - const char* - Pointer to an external string buffer
by method `publish`, it initiates the data item with method `init`.
* **void setValue(String value)**: The method stores the stringified value and if it differs from previously published one.
  * **value**: Stringified value of the data item.
* **String get()**: The getter returns the stringified data items's current value.
* **String publish()**: The method returns the stringified data items's current value with method `get`, stores the returned value as recently published one, and marks the item as used with the method `used`.
* **char *getName()**: The getter returns pointer to buffer with the data item's name.
* **Datatype getType()**: The getter returns the enumeration literal determining the original data type of the item.
* **bool isReady()**: The methods evaluates all current flags that have been set by following methods and decides whether the data item is available for publishing or other similar usage.
* **void reset([bool force = false])**: The method redefines the data item to unknown on with data type and default value for an unknown data type, but only if the input argument is true or the item is set for one-time usage with method `once`. The input argument is used usually by the item constructor only.
  * **force**: Flag determining that the data item should be reset in either case.
* **void init()**: It marks the data item that it can be used for publishing because its current value differs from recently published one.
* **void used()**: It is counterpart for the previous method and marks the data item that it has been used by the method `publish` and stores its recently publish value. It avoids from multiple publishing of the unchanged value if it is desirable.
* **void always()**: It marks the data item that it should be published regardless of the change of it value.
* **void change()**: It is counterpart for the previous method and marks the data item that it should be published only after changing its value from recently published one.
* **void show()**: It marks the data item that it is generally enabled for publishing.
* **void hide()**: It is counterpart for the previous method and marks the data item that it is generally disabled for publishing. It can be utilized for temporary suppressing the data item for particular conditions.
* **void once()**: It marks the data item that it is aimed for one-time publishing. A value should be set for the item in order to be published again. It is useful for suppressing publishing unknown (undefined) value right after creating the data item.
* **void more()**: It is counterpart for the previous method and marks the data item that it is aimed for repeating publishing.

[Back to interface](#interface)


<a id="gbj_appdata"></a>

## gbj_appdata()

#### Description
Constructor creates the class instance object and initiates internal resources.

#### Syntax
    gbj_appdata()

#### Parameters
None

#### Returns
Object performing data items management.

[Back to interface](#interface)


<a id="getPrmName"></a>

## getPrmName()

#### Description
The method returns a pointer to a buffer with data item name.

#### Syntax
    const char *getPrmName(const char *progmemPrmName)

#### Parameters
* **progmemPrmName**: Pointer to an internal buffer with copy of a data item name stored in the flash memory of the microcontroller.
  * *Valid values*: pointer to a constant array of characters
  * *Default value*: none

#### Returns
Pointer to a data item name.

[Back to interface](#interface)
