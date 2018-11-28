
import test_reader as FR






if __name__ == "__main__":
    extra = FR.Pair()
    extra.first = '1'
    extra.second = '2'
    buf = extra.to_fbs()

    extra1 = FR.Pair(buf)

    acc = FR.Account()
    acc.langs.append(FR.test_fbs.Language.Language.CHT)
    acc.langs.append(FR.test_fbs.Language.Language.CHS)
    acc.extras.append(extra1)
    acc.test_int = 12
    acc.test_str = 'erer'
    buf = acc.to_fbs()
    
    acc1 = FR.Account(buf)

    pass
    
    


