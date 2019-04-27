# Project Storm Glass
>A decent commodity management system

Prepared by
Cheung Chun Ho (3035601853)

# Problem Setting
 - The system is best given to reowned brands which in desperate need of backtracing logistics
## Target group: renowned brands
- Lots of data migration
  - A logging system is neccessary
- There is a need for premission management as well
## Problem statement: 
There are different status of an inventory that the system need to resolve
 - Keep track on import/export of merchandise
 - Give warning if certain types of goods is out-of-stock/expired
 - Monitor Inventory Access

In the meantime, the system shall be under control of authorized staff, which they can
 - Add/Delete Merchanise
 - Search for Merchandise according to different applied filters.
 - Update Existing Merchanise
 - View previous statistics (e.g. what type of commodity is restored the most)
 - Change currency according to the country of the provider of the commodity
 
## System Features
1. Search commodity according to different filters (e.g. in-stock/out-of-stock)
2. Insert new commodity with basic information (name, manufacturer, amount, price, availability of shop(s)) after eachh procurement
3. Delete obsolete commodity
4. Update commodity information
5. Automatic change of inventory status according to the amount of commodity that is currently available (e.g. when the amount reaches zero, the inventory status would become "out-of-stock")
6. Deliver alerts when the commodity is "out-of-stock"
7. Merchandise Statistics and permission

## System Design
To keep track on product import and export, the system we created has a logging system as the main component, which helps management staff to trace any changes made to the inventory. Secondly, the system is equiped with a permission system, users are defined into two groups for different degree of access to the system.
The ordinary users are able to search for existing commodities and change property of those goods. While the administrators can add and delete records in addition to the mentioned functions.
Thirdly, the system has a password lock embedded to the code itself, providing a basic form of secuirty to protect the privilege of the administrators.

Other than the specialised functions, the system provide ordinary functionality as a commodity management system.
For example:
 - Add/Delete Records
 - Editing of Records
 - Search for Records
 - Expiration Detection
 - View All Records
 
## Implementation
The system is build using C++, making use of File Streams, Time and Structures to implement the Inventory system.
The most important above all, is that datum in the files are formatted in a manner that the program will recognise and retrieve quickly.

### Example 1 of data files: USERS.TXT

	Alex A

Above is a line of user data stored in USER.TXT

The Left indicate the name of the user and the right is a indicator for administrative privileges, and that user will need to enter the admin password in order to login. The right side would be empty if that user is a ordinary user.

### Exmple 2 of data files: ITEMS.TXT

	Eggs,eggs,100,2,2019-4-25

The above line contains 5 information above the eggs just been imported. And it is in the following order.
	<Product Name>,<Product Category>,<Quantity>,<Shelf Life>,<Import Date>

The system will retrieve such information by seperating according to the commas.


## Instructions to compile
Clone this to your local directory and type make in you terminal, it will automatically link all source files and generate a bunch of new files, but the only executable is called main.

Type
	./main
to start using the system