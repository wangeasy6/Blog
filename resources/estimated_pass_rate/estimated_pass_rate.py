# -*- coding: UTF-8 -*-
# estimated_pass_rate.py

def actualPassedRate(total, passed):
    return passed/total

def estimatedConservativePassedRate(total, tested, passed):
    estimated_passed = passed/total * (total - tested)
    return (estimated_passed + passed) / total

def estimatedRadicalPassedRate(total, tested, passed):
    estimated_passed = passed/tested * (total - tested)
    return (estimated_passed + passed) / total

def estimatedBalancedPassedRate(total, tested, passed):
    estimated_passed = (passed/tested + passed/total) * (total - tested)
    estimated_passed /= 2
    return (estimated_passed + passed) / total

def actualFailedRate(total, failed):
    return failed/total

def estimatedConservativeFailedRate(total, tested, failed):
    estimated_failed = failed/total * (total - tested)
    return (estimated_failed + failed) / total

def estimatedRadicalFailedRate(total, tested, failed):
    estimated_failed = failed/tested * (total - tested)
    return (estimated_failed + failed) / total

def estimatedBalancedFailedRate(total, tested, failed):
    estimated_failed = (failed/tested + failed/total) * (total - tested)
    estimated_failed /= 2
    return (estimated_failed + failed) / total



if __name__ == "__main__":
    N = 100
    t = 55
    n = 55
    m = t - n

    print("总测试数：{}  已测试：{}  合格数：{}  不合格数：{}".format(N, t, n, m))

    print("实际合格率：{:.1%}".format( actualPassedRate(N, n) ))
    print("预估（保守）合格率：{:.1%}".format( estimatedConservativePassedRate(N, t, n) ))
    print("预估（激进）合格率：{:.1%}".format( estimatedRadicalPassedRate(N, t, n) ))
    print("预估（平衡）合格率：{:.1%}".format( estimatedBalancedPassedRate(N, t, n) ))

    print("实际不合格率：{:.1%}".format( actualFailedRate(N, m) ))
    print("预估（保守）不合格率：{:.1%}".format( estimatedConservativeFailedRate(N, t, m) ))
    print("预估（激进）不合格率：{:.1%}".format( estimatedRadicalFailedRate(N, t, m) ))
    print("预估（平衡）不合格率：{:.1%}".format( estimatedBalancedFailedRate(N, t, m) ))