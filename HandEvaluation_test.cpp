#include "Deck.h"
#include "HandEvaluation.h"
#include <gtest/gtest.h>

const std::array<Card, 7> royal_flush {
    Card{Suit::diamonds, Rank::ace}, Card{Suit::diamonds, Rank::king},
    Card{Suit::diamonds, Rank::queen}, Card{Suit::diamonds, Rank::jack},
    Card{Suit::diamonds, Rank::ten}, Card{Suit::spades, Rank::two},
    Card{Suit::spades, Rank::three}
};

const std::array<Card, 7> straight_flush {
    Card{Suit::diamonds, Rank::ten}, Card{Suit::diamonds, Rank::nine},
        Card{Suit::diamonds, Rank::eight}, Card{Suit::diamonds, Rank::seven},
    Card{Suit::diamonds, Rank::six}, Card{Suit::spades, Rank::two},
    Card{Suit::spades, Rank::three}
};

const std::array<Card, 7> four_of_a_kind {
    Card{Suit::clubs, Rank::two}, Card{Suit::spades, Rank::two},
    Card{Suit::diamonds, Rank::two}, Card{Suit::hearts, Rank::two},
    Card{Suit::diamonds, Rank::six}, Card{Suit::spades, Rank::jack},
    Card{Suit::spades, Rank::three}
};

const std::array<Card, 7> full_house {
    Card{Suit::clubs, Rank::king}, Card{Suit::spades, Rank::king},
    Card{Suit::diamonds, Rank::king}, Card{Suit::spades, Rank::seven},
    Card{Suit::diamonds, Rank::seven}, Card{Suit::hearts, Rank::two},
    Card{Suit::clubs, Rank::two}
};

const std::array<Card, 7> flush {
    Card{Suit::clubs, Rank::ace}, Card{Suit::clubs, Rank::king},
    Card{Suit::clubs, Rank::queen}, Card{Suit::clubs, Rank::jack},
    Card{Suit::clubs, Rank::nine}, Card{Suit::hearts, Rank::two},
    Card{Suit::hearts, Rank::three}
};

const std::array<Card, 7> straight {
    Card{Suit::clubs, Rank::king}, Card{Suit::clubs, Rank::queen},
    Card{Suit::spades, Rank::jack}, Card{Suit::clubs, Rank::ten},
    Card{Suit::clubs, Rank::nine}, Card{Suit::hearts, Rank::two},
    Card{Suit::hearts, Rank::three}
};

const std::array<Card, 7> three_of_a_kind {
    Card{Suit::clubs, Rank::ace}, Card{Suit::spades, Rank::ace},
    Card{Suit::diamonds, Rank::ace}, Card{Suit::clubs, Rank::jack},
    Card{Suit::clubs, Rank::ten}, Card{Suit::hearts, Rank::two},
    Card{Suit::hearts, Rank::three}
};

const std::array<Card, 7> two_pair {
    Card{Suit::clubs, Rank::ace}, Card{Suit::spades, Rank::ace},
    Card{Suit::diamonds, Rank::king}, Card{Suit::clubs, Rank::king},
    Card{Suit::clubs, Rank::ten}, Card{Suit::hearts, Rank::two},
    Card{Suit::hearts, Rank::three}
};

const std::array<Card, 7> one_pair {
    Card{Suit::clubs, Rank::ace}, Card{Suit::spades, Rank::ace},
    Card{Suit::diamonds, Rank::king}, Card{Suit::clubs, Rank::jack},
    Card{Suit::clubs, Rank::ten}, Card{Suit::hearts, Rank::two},
    Card{Suit::hearts, Rank::three}
};

const std::array<Card, 7> high_card {
    Card{Suit::clubs, Rank::ace}, Card{Suit::spades, Rank::ten},
    Card{Suit::diamonds, Rank::jack}, Card{Suit::clubs, Rank::five},
    Card{Suit::clubs, Rank::eight}, Card{Suit::hearts, Rank::two},
    Card{Suit::hearts, Rank::three}
};

TEST(HandEvaluationTest, HandEvaluator_RoyalFlush_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(royal_flush);
    EXPECT_EQ(HandType::royal_flush, static_cast<HandType>(hand_sh >> 20));
    EXPECT_EQ(0, hand_sh & 0xFFFFF);
}

TEST(HandEvaluationTest, HandEvaluator_StraightFlush_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(straight_flush);
    EXPECT_EQ(HandType::straight_flush, static_cast<HandType>(hand_sh >> 20));

    std::array<Rank, 5> found_ranks = get_ranks_from_hand_sh(hand_sh);
    for (int i {0}; i < 5; ++i) {
        EXPECT_EQ(straight_flush[i].rank, found_ranks[i]);
    }
}

TEST(HandEvaluationTest, HandEvaluator_FourOfAKind_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(four_of_a_kind);
    EXPECT_EQ(HandType::four_of_a_kind, static_cast<HandType>(hand_sh >> 20));

    const std::array<Rank, 5> found_ranks {get_ranks_from_hand_sh(hand_sh)};
    for (int i {0}; i < 4; ++i) {
        EXPECT_EQ(four_of_a_kind[i].rank, found_ranks[i]);
    }
    EXPECT_EQ(four_of_a_kind[5].rank, found_ranks[4]);
}

TEST(HandEvaluationTest, HandEvaluator_FullHouse_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(full_house);
    EXPECT_EQ(HandType::full_house, static_cast<HandType>(hand_sh >> 20));

    std::array<Rank, 5> found_ranks {get_ranks_from_hand_sh(hand_sh)};
    for (int i {0}; i < 5; ++i) {
        EXPECT_EQ(full_house[i].rank, found_ranks[i]);
    }
}

TEST(HandEvaluationTest, HandEvaluator_Flush_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(flush);
    EXPECT_EQ(HandType::flush, static_cast<HandType>(hand_sh >> 20));

    const std::array<Rank, 5> found_ranks {get_ranks_from_hand_sh(hand_sh)};
    for (int i {0}; i < 5; ++i) {
        EXPECT_EQ(flush[i].rank, found_ranks[i]);
    }
}

TEST(HandEvaluationTest, HandEvaluator_Straight_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(straight);
    EXPECT_EQ(HandType::straight, static_cast<HandType>(hand_sh >> 20));

    const std::array<Rank, 5> found_ranks {get_ranks_from_hand_sh(hand_sh)};
    for (int i {0}; i < 5; ++i) {
        EXPECT_EQ(straight[i].rank, found_ranks[i]);
    }
}

TEST(HandEvaluationTest, HandEvaluator_ThreeOfAKind_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(three_of_a_kind);
    EXPECT_EQ(HandType::three_of_a_kind, static_cast<HandType>(hand_sh >> 20));

    const std::array<Rank, 5> found_ranks {get_ranks_from_hand_sh(hand_sh)};
    for (int i {0}; i < 5; ++i) {
        EXPECT_EQ(three_of_a_kind[i].rank, found_ranks[i]);
    }
}

TEST(HandEvaluationTest, HandEvaluator_TwoPair_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(two_pair);
    EXPECT_EQ(HandType::two_pair, static_cast<HandType>(hand_sh >> 20));

    const std::array<Rank, 5> found_ranks {get_ranks_from_hand_sh(hand_sh)};
    for (int i {0}; i < 5; ++i) {
        EXPECT_EQ(two_pair[i].rank, found_ranks[i]);
    }
}

TEST(HandEvaluationTest, HandEvaluator_OnePair_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(one_pair);
    EXPECT_EQ(HandType::one_pair, static_cast<HandType>(hand_sh >> 20));

    const std::array<Rank, 5> found_ranks {get_ranks_from_hand_sh(hand_sh)};
    for (int i {0}; i < 5; ++i) {
        EXPECT_EQ(one_pair[i].rank, found_ranks[i]);
    }
}

TEST(HandEvaluationTest, HandEvaluator_HighCard_CorrectHandSh) {
    const uint32_t hand_sh = get_hand_sh(high_card);
    EXPECT_EQ(HandType::high_card, static_cast<HandType>(hand_sh >> 20));

    const std::array<Rank, 5> found_ranks {get_ranks_from_hand_sh(hand_sh)};
    for (int i {0}; i < 5; ++i) {
        EXPECT_EQ(high_card[i].rank, found_ranks[i]);
    }
}