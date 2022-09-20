with open('data') as i:
    with open('data.s', 'w') as o:
        o.write('.data\ndata:\n')
        for c in i.read():
            o.write(f'\t.byte {ord(c)}\n')
        o.write('\t.byte 0\n')
