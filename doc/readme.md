### CMD

##### common

1. keys

2. affair

3. commit

4. rollback

5. discard

##### string

1. set [key] [value]

2. get [key]

3. delete [key]

4. exist [key]

5. len [key]

6. expire [key] [seconds]

7. ttl [key]

##### vector

1. vappend [key] [value...]

2. vget [key]

3. vdelete [key]

4. vindx [key] [value]

5. verase [key] [indx]

6. vinsert [key] [indx] [value]

6. vexist [key] ([value])

7. vsize [key]

8. vexpire [key] [seconds]

9. vttl [key]

##### list

1. lget [key]

2. ldelete [key]

3. lerase [key] [value...]

4. lexist [key] ([value])

5. lpush [key] [value...]

6. rpush [key] [value...]

7. lsize [key]

8. lexpire [key] [seconds]

9. lttl [key]

##### rbtree

1. rbappend [key] {[field] [value] ...}

2. rbget [key] [field...] 

3. rbdelete [key]

4. rberase [key] [field...]

5. rbexist [key] ([value])

6. rbsize [key]

7. rbkeys [key]

8. rbexpire [key] [seconds]

9. rbttl [key]

##### hash

1. happend [key] [value...]

2. hget [key]

3. hdelete [key]

4. herase [key] [value...]

5. hexist [key] ([value])

6. hsize [key]

7. hexpire [key] [seconds]

8. httl [key]