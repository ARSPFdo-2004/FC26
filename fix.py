import re
with open('marketplace.c', 'r') as f:
    c = f.read()

c = re.sub(r'%-10s\\n",\s*"Name",\s*"Type",\s*"Rating",\s*"Price",\s*"Seller ID"', '%s\\n", "Name", "Type", "Rating", "Price"', c)
c = re.sub(r'%-10s\\n",\s*"---",\s*"-------------------------",\s*"-----",\s*"-------",\s*"--------",\s*"----------"', '%s\\n", "---", "-------------------------", "-----", "-------", "--------"', c)
c = re.sub(r'%-10d\\n",\s*i \+ 1,\s*current -> name,\s*current -> type,\s*current -> rating,\s*current -> price,\s*current -> seller_id', '%d\\n", i + 1, current -> name, current -> type, current -> rating, current -> price', c)

# more specific regex:
# Just fix all seller_id occurences:
c = re.sub(r',\s*current\s*->\s*seller_id\)', ')', c)
c = re.sub(r' %-10s', '', c)
c = re.sub(r' %-10d', '', c)

c = re.sub(r'"Seller ID"\)', '""\)', c) # Wait, this might break things. I will just rewrite those functions cleanly.
