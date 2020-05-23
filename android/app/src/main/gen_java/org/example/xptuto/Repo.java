// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from main.djinni

package org.example.xptuto;

import java.util.Date;

public final class Repo {


    /*package*/ final int id;

    /*package*/ final String name;

    /*package*/ final String fullName;

    /*package*/ final int owner;

    /*package*/ final boolean priv;

    /*package*/ final String descr;

    /*package*/ final Date createdAt;

    public Repo(
            int id,
            String name,
            String fullName,
            int owner,
            boolean priv,
            String descr,
            Date createdAt) {
        this.id = id;
        this.name = name;
        this.fullName = fullName;
        this.owner = owner;
        this.priv = priv;
        this.descr = descr;
        this.createdAt = createdAt;
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }

    public String getFullName() {
        return fullName;
    }

    public int getOwner() {
        return owner;
    }

    public boolean getPriv() {
        return priv;
    }

    public String getDescr() {
        return descr;
    }

    public Date getCreatedAt() {
        return createdAt;
    }

    @Override
    public String toString() {
        return "Repo{" +
                "id=" + id +
                "," + "name=" + name +
                "," + "fullName=" + fullName +
                "," + "owner=" + owner +
                "," + "priv=" + priv +
                "," + "descr=" + descr +
                "," + "createdAt=" + createdAt +
        "}";
    }

}