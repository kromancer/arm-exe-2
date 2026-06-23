/*
 * SPDX-FileCopyrightText: Copyright 2023 Arm Limited and/or its affiliates
 * <open-source-office@arm.com> SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*****************************************************************************
 * Includes
 *****************************************************************************/

#include "menu.hpp"

/*****************************************************************************
 * Dishes
 *****************************************************************************/

Dish::Dish(const std::string &_name, unsigned _price)
    : name{_name}, price{_price} {}

std::ostream& Dish::print(std::ostream &out) const {
    return out << name << ", price: " << price;
};

bool Dish::isSameName(const Dish &other) const {
    return name == other.name;
}

Pizza::Pizza(const std::string &name, unsigned price) : Dish(name, price) {}

bool Pizza::matches(const Dish &d) const {
    const auto *pizza = dynamic_cast<const Pizza *>(&d);
    return pizza && isSameName(d);
}

Burger::Burger(const std::string &name, unsigned _weight, unsigned price)
    : Dish(name, price), weight{_weight} {}

std::ostream& Burger::print(std::ostream &out) const {
    return Dish::print(out) << ", weight: " << weight;
}

bool Burger::matches(const Dish &d) const {
    const auto *burger = dynamic_cast<const Burger *>(&d);
    // weight 0 means "unspecified", matches any weight.
    return burger && isSameName(d) && (burger->weight == 0 || weight == burger->weight);
}

Softdrink::Softdrink(const std::string &name, unsigned _volume, unsigned price)
    : Dish(name, price), volume{_volume} {}

std::ostream& Softdrink::print(std::ostream &out) const {
    return Dish::print(out) << ", volume: " << volume;
}

bool Softdrink::matches(const Dish &d) const {
    const auto *softdrink = dynamic_cast<const Softdrink *>(&d);
    // volume 0 means "unspecified", matches any volume.
    return softdrink && isSameName(d) && (softdrink->volume == 0 || volume == softdrink->volume);
}

IceCream::IceCream(const std::string &name, unsigned price)
    : Dish(name, price) {}

bool IceCream::matches(const Dish &d) const {
    const auto *ice = dynamic_cast<const IceCream *>(&d);
    return ice && isSameName(d);
}

/*****************************************************************************
 * Order
 *****************************************************************************/

void Order::operator+=(const Order &other) {
    dishes.insert(dishes.end(), other.dishes.begin(), other.dishes.end());
}

void Order::operator+=(const std::shared_ptr<Dish> &other) {
    dishes.push_back(other);
}

unsigned Order::getTap() const {
    unsigned tap = 0;
    for (const auto &dish : dishes) {
        tap += dish->getPrice();
    }
    return tap;
}

std::ostream &operator<<(std::ostream &out, const Order &order) { return out; }

/*****************************************************************************
 * Menu
 *****************************************************************************/

Menu::Menu(const Menu &other) : dishes(other.dishes) {}

Menu::Menu(Menu &&other) noexcept : dishes(std::move(other.dishes)) {}

Menu::Menu(std::initializer_list<std::shared_ptr<Dish>> l) : dishes(l) {}

Menu& Menu::operator=(const Menu &other) {
    dishes = other.dishes;
    return *this;
}

Menu& Menu::operator=(Menu &&other) noexcept {
    dishes = std::move(other.dishes);
    return *this;
}

size_t Menu::size() const { return dishes.size(); }

bool Menu::isAvailable(const Dish &dish) const {
  return findDish(dish) != dishes.end();
}

std::vector<std::shared_ptr<Dish>>::const_iterator Menu::findDish(const Dish &dish) const {
    auto best = dishes.end();
    for (auto it = dishes.begin(); it != dishes.end(); ++it) {
        if (!(*it)->matches(dish))
            continue;
        // Given behavior in unit test, if more than one dish matches a given request, pick the cheapest one
        if (best == dishes.end() || (*it)->getPrice() < (*best)->getPrice())
            best = it;
    }
    return best;
}

std::ostream& operator<<(std::ostream &out, const Menu &menu) {
    for (const auto& dish : menu.dishes) {
        dish->print(out) << std::endl;
    }
    return out;
}
