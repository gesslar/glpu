private nosave mapping items = ([]);

int set_items(mapping itemData);
int remove_item(string id);
int add_item(mixed id, string desc);

int set_items(mapping itemData)
{
    mixed id;
    string key;
    mapping item = ([]);

    foreach(id, key in itemData)
    {
        if(pointerp(id))
        {
            int i;
            for(i = 0; i < sizeof(id); i++)
                item += ([id[i] : key]);
        }
        else item += ([id : key]);
    }

    items = item;
    return 1;
}

mapping query_items()
{
    mapping copy;
    copy = items;
    return copy;
}

string query_item(string id)
{
    if(items[id])
    {
        if(items[id][0..1] == "@@") return( call_other(this_object(), items[id][2..(strlen(items[id])-1)]));
        else return items[id];
    }
}


int remove_item(string id)
{
    if(!items[id]) return 1;
    map_delete(items, id);
    return 1;
}

int add_item(mixed id, string desc)
{
    if(arrayp(id))
    {
        int i;

        for(i = 0; i < sizeof(id); i++)
        {
            if(items[id[i]]) map_delete(items, id);
            items += ([id[i] : desc]);
        }
    }
    else
    {
        if(items[id]) map_delete(items, id);
        items += ([id : desc]);
    }
    return 1;
}
