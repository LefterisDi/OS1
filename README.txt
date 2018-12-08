**************************************************************************************************
->GENERAL :
    
    HOW TO COMPILE & RUN :
    In order to compile the program use make .
    In order to delete every executable use make clean.
    In order to run the program use "./sim -N (number of items per construction dep)"
    e.x. ./sim -N 1000
    
    MERC ID & FINAL PRODUCT ID :
    The Mercs created by the construction deps take an ID created like so :
    "1000 + i + numOfMercs*merc->type"  where "i" is a variable used in for loop to indicate the
    number of loops . This happens so that no Merc has got the same ID . Lastly the final product
    is a 12-character ID.
    e.x. Merc1.ID = 1001 , Merc2.ID = 1002 , Merc3.ID = 1003 -> finalMerc.ID = 100110021003
    
    TIME :
    Time is calculated using gettime.
    Time spent in departments for proccessing is maximum 10 ms.
**************************************************************************************************
->SHARED MEMORY :
    The program uses queues in shared memory in order to transfer Merc data from one proccess
    to another . The structure of these queues contains an array . This array is allocated using
    a second shared memory . That is because we do not know the original size of this array , 
    since we do not initially know how many Mercs per construction department we need to build.
    So a Merc* is used in the data struct of the Queue , which leads us to this allocation.
    
**************************************************************************************************
->SEMAPHORES :
    Now as for the semaphores we use 14 of them . 
    
    7 of them act as counters for the queues we have created. 
    E.x. The construction dep 1 makes a Merc and inserts it into the queue
    between the construction deps and the paint dep . The counter semaphore value is 0 , so
    the construction dep 1 'ups' it in order for the semaphore to get the value of 1 , meaning
    that there is exactly 1 item in the queue . Now when the paint dep receives this item it
    'downs' the semaphore to indicate that an item has been removed from the queue (the semaphore
    value may vary according to the items inserted in the queue) . All of the above is done in 
    order for the paint dep to wait for an item before starting any proccedures.
    
    The other 7 semaphores are used in order to block all other proccesses from using the queue
    when a proccess is already using it.
    E.x. The construction dep 1 makes a Merc and wants to insert it into the queue. It tries
    to 'down' the semaphore . If it succeeds then it can insert its item into the queue . 
    Now while construction dep 1 is using the queue , the other departments that want to 
    either store or take items from the queue cannot do it since they are blocked by the
    semaphore (since its value is 0 , they cannot 'down' it . They just have to wait).
    
**************************************************************************************************

->RESULT :
    
    N = 100 :(measured with time took around 2 seconds real-time to complete)
    1st run
    Average Waiting Time for painting department(in ms) : 1087.072383
    Average Completion Time per product(in ms) : 1097.941760
    
    2nd run
    Average Waiting Time for painting department(in ms) : 1093.926820
    Average Completion Time per product(in ms) : 1122.898390

    3rd run
    Average Waiting Time for painting department(in ms) : 1093.032723
    Average Completion Time per product(in ms) : 1129.307130

    
    
    N = 1000 :(measured with time took around 21 seconds real-time to complete)
    1st run
    Average Waiting Time for painting department(in ms) : 10843.497787
    Average Completion Time per product(in ms) : 10931.360015
    
    2nd run
    Average Waiting Time for painting department(in ms) : 10878.342808
    Average Completion Time per product(in ms) : 10934.152909

    3rd run
    Average Waiting Time for painting department(in ms) : 10913.132623
    Average Completion Time per product(in ms) : 10943.688661
    
    N = 2000 :(measured with time took around 43 seconds real-time to complete)
    1st run
    Average Waiting Time for painting department(in ms) : 21764.046923
    Average Completion Time per product(in ms) : 21842.024058
    
    2nd run
    Average Waiting Time for painting department(in ms) : 21783.880464
    Average Completion Time per product(in ms) : 21915.839996
    
    3rd run
    Average Waiting Time for painting department(in ms) : 21778.194845
    Average Completion Time per product(in ms) : 21895.627945
    
    N = 10000 : (measured with time took around 3 minutes and 38 seconds seconds real-time to complete)
    THIS STUNT WAS PULLED OFF BY SPECIALLY TRAIN... EEM I MEAN CREATED CPU CORES . PLEASE DO NOT TRY THIS AT HOME . 
    
    1st run
    Average Waiting Time for painting department(in ms) : 109076.232273
    Average Completion Time per product(in ms) : 109517.472508
    
    2nd run
    Average Waiting Time for painting department(in ms) : 109434.766466
    Average Completion Time per product(in ms) : 110290.598958


    3rd run
    Average Waiting Time for painting department(in ms) : 109360.234292
    Average Completion Time per product(in ms) : 109575.878145




    



    
