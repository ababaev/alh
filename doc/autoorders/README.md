# alh (Atlantis Little Helper)
## AutoOrders:
Feature was designed to automate caravans and production. 
There is an order to determine unit as a source of item. And order to determine unit as the need of item with priority.
Also a way to determine caravans, which will be able to collect items from sources for needs in regions where this caravan is heading.

### List of orders:

#### SOURCE
Source order: `@;!SOURCE X ITEM`
Source order with priority: `@;!SOURCE X ITEM P Y`

It means that this unit should be considered as a source of item `ITEM`. Amount of available items is calculated as `current amount` of unit's items minus `X`. `X` have to be non-negative.
If source order has priority `Y`, it will be used just for need requests with priority `Z` where `Z < Y`. `Y` have to be non-negative.

##### Example 1
unit with `10 WOOD` has order `@;!SOURCE 4 WOOD`, this means it may give out up to 6 (10-4) items of WOOD to any unit which NEED it.

##### Example 2
unit with `10 WOOD` has order `@;!SOURCE 15 WOOD`, this means it will not give out any WOOD this turn.

##### Example 3
Example3: unit with `10 WOOD` has order `@;!SOURCE 0 WOOD P 11`, this means it may give out up to 10 items of WOOD but just to units with priority of NEED below 11.

#### NEED
Need order: `@;!NEED X ITEM P Y`
It means, that this unit needs `X` items `ITEM` with priority `Y`.
if `X` >= 0, it represents finite amount of items which should be requested.
if `X` == -1, it considered as infinite request of items. All other values of `X` are invalid.
`Y` have to be non-negative. The lower `Y`, the higher priority.
If `P Y` wasn't specified, default priority of `X` >= 0 is 10. Default priority of `X` == -1 is 20.

##### Example 1
unit with `10 WOOD` has order `@;!NEED 4 WOOD`, this means it will not try to receive any items this turn, because it already has more. Request has priority 10.

##### Example2
unit with `10 WOOD` has order `@;!NEED 15 WOOD P 15`, this means it will need 5 (5, because it already has 10) WOOD with priority 15.

##### Example3 
unit with `10 WOOD` has order `@;!NEED -1 WOOD P 25`, this means it need as many wood as it can receive with priority 25.

#### STORE
Store order: `@;!STORE X ITEM P Y`
It means, that this unit needs `-1`(ALL) items `ITEM` with priority `Y` and at the same time source them after amount `X` with specified priority.
if `X` >= 0, it's absolutely equal to combination of two commands: `NEED -1 ITEM P Y` && `SOURCE X ITEM P Y`.
if `X` == -1, it considered as combination of `NEED -1 ITEM P Y` & `SOURCE 0 ITEM P Y`.

#### STORE_ALL
Store all order: `@;!STORE_ALL P Y`
Unit will try to store ANY item, which is not SILV or PRP_MEN with specified priority. As much as it can.
Also it will SOURCE all items it has, except PRP_MEN & SILV, with specified priority.

#### EQUIP
Equip order: `@;!EQUIP ITEM1 ITEM2 ITEM3 P X`
It means, that this unit will need those items for each man in unit. 
For example, unit with 13 orcs and `@;!EQUIP SWOR MARM ISHD P 9` is the same as having next set of commands:
`@;!NEED 13 SWOR P 9`, `@;!SOURCE 13 SWOR P 9`,
`@;!NEED 13 MARM P 9`, `@;!SOURCE 13 MARM P 9`, 
`@;!NEED 13 ISHD P 9`, `@;!SOURCE 13 ISHD P 9`

#### NEEDREG
Need region order: `@;!NEEDREG X ITEM P Y`
It means same as need, but it takes into account all `ITEM` in region, but not just items of current unit.
Important: Caravan's are not counted as units of region.

##### Example 1
unit with `1000 SILV` has order `@;!NEEDREG 3000 SILV`. And in the same region there is another unit of this faction, which has `1500 SILV`. Then actual demand will be `500 SILV` (3000 - 1000 - 1500)

#### CARAVAN
Caravan definition order: `@;!CARAVAN SPEED` 
It means that this unit is considered as caravan with speed at least equal to `SPEED`. 
`SPEED` may take next values: `MOVE`|`WALK`|`RIDE`|`FLY`|`SWIM`. 
This means that it is a caravan, which will try to load itself being able to move at least with specified speed.

Each item in unit which is CARAVAN is considered as a SOURCE. To preserve items in caravan it needs to add him `@;!NEED X ITEM` order.
Example: unit has next orders: `@;!CARAVAN RIDE`, `@;!NEED 2 WELF`, `@;!NEED 10 HORS`. This means all items of this unit may be given out if there will be requests, except `2 WELF` and `10 HORS`.

##### Example 1
`@;!CARAVAN MOVE` means it is a caravan, and it's moving type should be at least MOVE. It will not load item if it lose ability to move.

##### Example 2
`@;!CARAVAN WALK` same as MOVE.

##### Example 3
`@;!CARAVAN RIDE` means it is a caravan, and it's moving type should be at least RIDE. It will not load item if it lose ability to ride.

##### Example 4
`@;!CARAVAN` means it is a caravan, which doesn't have any movement restriction. Can be used to work as a local store of items, for example.


#### REGION
List of regions definition order: `@;!REGION X1,Y1,Z1 X2,Y2,Z2 ...` 
It has meaning just if it is a caravan (it has CARAVAN order). 
It consider regions listed in `@;!REGION` as target regions for caravan. 
X,Y,Z -- are coordinates of region, where Z -- is number of level (nexus = 0, surface = 1 and so on, but your ALH may be set up differently)

Caravan will behave as caravan JUST in regions, listed in `@;REGION` command.

##### Example 1
`@;!REGION 15,17,1 20,22,1` -- means that if it is a caravan, and it is located in `15,17,1`, then it will try to load items according to needs of `20,22,1`. 
If it is located in `20,22,1`, it will try to load items according to needs of `15,17,1`. If it's located in any else region, it will try to load items according to needs of both those regions.


### How does it work.

To be clear, I'll descrive steps.
- clean all unit's orders marked as `;!ao` -- this is a mark of orders, generated by autoorders.

- run current orders, so the state of ALH will take into account all orders, except autogenerated.

- move over regions one by one (from left to right, from top to bottom), so if caravan is heading to already processed region, it will not take into account fulfilled needs of the region. And vice versa, if it is heading to region which was not processed, it may take needs.

- within each region:
    * generate map of all sources (including units with `@;!SOURCE` order and caravans).
    * generate map of needs (from units with `@;!NEED`)
    * generate needs of caravans. If current region is listed in `REGION` of caravan, it will generate it needs by iterating over each `REGION` (except current) in list of regions, and if there are needs, this caravan would get get similar `NEED` request for remote region.
    * sort all `NEED` requests according to their priority.
    * iterate over `NEED` requests, trying to fulfill them from `SOURCE` if possible. If current `NEED` request belong to caravan, then it's `SPEED` and amount of possible weight is taken into account.
    * for each positive amount of items which may be given from `SOURCE` to `NEED`, generate `GIVE` order with comment `;!ao`, as a sign of autogenerated order.

### Recomendations

#### Design
In fact, we can have 4 type of units involved into AutoOrders.
* Distributor -- a unit with one or few `@;!SOURCE` orders. Is a source of specified items.
* Collector -- a unit with one or few `@;!NEED` orders. Is requesting specified items.

Distributor and Collector may be located in the same region, then item may be bypassed between them directly (according to priorities).
If Distributor and Collector located in different regions, then they need a Caravan.

* Caravan -- a unit with `@;!CARAVAN` & `@;!REGION` orders. It should move between specified regions and distribute thems among them.
* Storage -- a unit with `@;!NEED` and `@;!SOURCE` orders related to the same item (or just `@;!STORE`, which works the same). Such unit can collect and redistribute items.

Because rarely we can calculate exact amount of items, it's a good practice to have a Storage with low priority in each region with Collector, so Storage would get items after the Collector, and may give them him later, if there will be lack of them. Or bypass them to another caravan, heading somewhere else.

#### Tips
* don't set up two caravans moving simultaneusly over one route. For each `NEED` from target region each of those caravans would generate it's own need, so amount of items transfered there would be doubled.

* it may happen that item will be transfered to caravan, but not to local unit, if `NEED` of a unit in target caravan's region has higher priority. But that's the goal. If you have sequence of caravans, then items from one caravan may be taken by another caravan instead of giving/taking to/from storage.

#### Full Example
##### Initial state
<Region 24,24>
===
;Unit, producing livestock
@;!SOURCE 0 LIVE
===
;Unit, producing iron
@;!SOURCE 0 IRON

<Region 24,26>
===
;Unit, producing 4 parm (number 1515)
@;!NEED 12 IRON P 21
===
;Unit, storing the rest iron (number 1516)
@;!STORE -1 IRON P 24
@;!STORE -1 LIVE P 25
@;SELL ALL LIVE ;!COND ITEM[LIVE] > 0

<Caravan 1514>
@;!CARAVAN MOVE
@;!REGION 24,24 24,26
@;!NEED 10 HORS

##### When caravan in 24,24
<Region 24,24>
===
;Unit, producing livestock
@;!SOURCE 0 LIVE
GIVE 1514 14 LIVE ;!ao Reg[24,26,1][-1p25]
===
;Unit, producing iron
@;!SOURCE 0 IRON
GIVE 1514 12 IRON ;!ao Reg[24,26,1][12p21]
GIVE 1514 13 IRON ;!ao Reg[24,26,1][-1p24]
===
;Caravan
MOVE S

<Region 24,26>
===
;Unit, producing 4 parm (number 1515)
@;!NEED 12 IRON P 21
===
;Unit, storing the rest iron (number 1516)
@;!STORE -1 IRON P 24
@;!STORE -1 LIVE P 25
@;SELL ALL LIVE ;!COND ITEM[LIVE] > 0
GIVE 1515 12 IRON ;!ao

###### Explanation
In 24,24 caravan analyzed requests of 24,26 and generated needs request according to them.
According to requests producers got orders to give items to caravan (with short description, which may be helpful to understand meaning of each give order).
Caravan generates move orders automatically, using shortest path.

In 24,26 storage which has IRON & stores it with priority 24 will give it out to producers (because producers request has priority 21).

##### When caravan in 24,26
<Region 24,24>
===
;Unit, producing livestock
@;!SOURCE 0 LIVE
===
;Unit, producing iron
@;!SOURCE 0 IRON

<Region 24,26>
===
;Unit, producing 4 parm (number 1515)
@;!NEED 12 IRON P 21
===
;Unit, storing the rest iron (number 1516)
@;!STORE -1 IRON P 24
@;!STORE -1 LIVE P 25
@SELL ALL LIVE ;!COND ITEM[LIVE] > 0
===
;Caravan
MOVE N
GIVE 1515 12 IRON ;!ao
GIVE 1516 13 IRON ;!ao
GIVE 1516 14 LIVE ;!ao

###### Explanation
In 24,26 caravan spread items it has according to requests: first 12 to producers, the rest to storage.
Also it gives all LIVE to storage.
Storage, because `!COND` evaluates to TRUE, uncomment sell order, and can sell LIVE.

In 24,24 nothing specific.

